package main

import (
	"fmt"
	"os"
	"strconv"
	"unicode"
)

type Tag int

const (
	ERROR  Tag = 1 << iota // Неправильная лексема
	NUMBER                 // Целое число
	VAR                    // Имя переменной
	PLUS                   // Знак +
	MINUS                  // Знак -
	MUL                    // Знак *
	DIV                    // Знак /
	LPAREN                 // Левая круглая скобка
	RPAREN                 // Правая круглая скобка
)

type Lexem struct {
	Tag   Tag
	Image string
}

type Parser struct {
	lexems chan Lexem
	curr   Lexem
	vars   map[string]int
}

func NewParser(lexems chan Lexem) *Parser {
	p := &Parser{lexems: lexems, vars: make(map[string]int)}
	p.nextLexem()
	return p
}

func (p *Parser) nextLexem() {
	if lx, ok := <-p.lexems; ok {
		p.curr = lx
	} else {
		p.curr = Lexem{ERROR, "EOF"}
	}
}

func (p *Parser) accept(tag Tag) bool {
	if p.curr.Tag == tag {
		return true
	}
	return false
}
func (p *Parser) accept2(tag Tag) bool {
	if p.curr.Tag == tag {
		p.nextLexem()
		return true
	}
	return false
}

func (p *Parser) expect(tag Tag) {
	if !p.accept2(tag) {
		fmt.Println("error")
		return
	}
}

func (p *Parser) E() (int, bool) {
	val, ok := p.T()
	if !ok {
		return 0, false
	}
	for p.curr.Tag == PLUS || p.curr.Tag == MINUS {
		if p.curr.Tag == PLUS {
			p.nextLexem()
			tmpVal, ok := p.T()
			if !ok {
				return 0, false
			}
			val += tmpVal
		} else {
			p.nextLexem()
			tmpVal, ok := p.T()
			if !ok {
				return 0, false
			}
			val -= tmpVal
		}
	}
	if p.accept(ERROR) && p.curr.Image != "EOF" {
		fmt.Println("error")
		return 0, false
	}
	return val, true
}

func (p *Parser) T() (int, bool) {
	val, ok := p.F()
	if !ok {
		return 0, false
	}
	for p.curr.Tag == MUL || p.curr.Tag == DIV {
		if p.curr.Tag == MUL {
			p.nextLexem()
			tmpVal, ok := p.F()
			if !ok {
				return val, false
			}
			val *= tmpVal
		} else {
			p.nextLexem()
			tmpVal, ok := p.F()

			if !ok || tmpVal == 0 {
				fmt.Println("error")
				return 0, false
			}
			val /= tmpVal
		}
	}
	if p.accept(ERROR) && p.curr.Image != "EOF" {
		fmt.Println("error")
		return 0, false
	}
	return val, true
}

func (p *Parser) F() (int, bool) {
	if p.accept(NUMBER) {
		val, err := strconv.Atoi(p.curr.Image)
		if err != nil {
			fmt.Println("error")
			return 0, false
		}
		p.nextLexem()
		if p.accept(PLUS) || p.accept(MINUS) || p.accept(MUL) || p.accept(DIV) || p.accept(RPAREN) || p.curr.Image == "EOF" {
			return val, true
		}
		fmt.Println("error")
		return 0, false
	} else if p.accept(VAR) {
		varName := p.curr.Image
		if val, ok := p.vars[varName]; ok {
			p.nextLexem()
			if p.accept(PLUS) || p.accept(MINUS) || p.accept(MUL) || p.accept(DIV) || p.accept(RPAREN) || p.curr.Image == "EOF" {
				return val, true
			}
			fmt.Println("error")
			return 0, false
		}
		varValStr := ""
		fmt.Scanln(&varValStr)
		p.nextLexem()
		varVal, err := strconv.Atoi(varValStr)
		if err != nil {
			fmt.Println("error")
			return 0, false
		}
		p.vars[varName] = varVal
		if p.accept(PLUS) || p.accept(MINUS) || p.accept(MUL) || p.accept(DIV) || p.accept(RPAREN) || p.curr.Image == "EOF" {
			return varVal, true
		}
		fmt.Println("error")
		return 0, false
	} else if p.accept(LPAREN) {
		p.nextLexem()
		tmpVal, ok := p.E()
		if !ok {
			return 0, false
		}
		p.expect(RPAREN)
		return tmpVal, true
	} else if p.accept(MINUS) {
		p.nextLexem()
		tmpVal, ok := p.F()
		if !ok {
			return 0, false
		}
		return -tmpVal, true
	} else {
		fmt.Println("error")

		return 0, false // to satisfy compiler, this won't be reached
	}
}

func lexer(expr string, lexems chan Lexem) {
	runes := []rune(expr)
	for i := 0; i < len(runes); i++ {
		switch {
		case unicode.IsDigit(runes[i]):
			numStr := string(runes[i])
			for i+1 < len(runes) && unicode.IsDigit(runes[i+1]) {
				i++
				numStr += string(runes[i])
			}
			lexems <- Lexem{NUMBER, numStr}
		case unicode.IsLetter(runes[i]):
			varStr := string(runes[i])
			for i+1 < len(runes) && (unicode.IsLetter(runes[i+1]) || unicode.IsDigit(runes[i+1])) {
				i++
				varStr += string(runes[i])
			}
			lexems <- Lexem{VAR, varStr}
		case runes[i] == '+':
			lexems <- Lexem{PLUS, "+"}
		case runes[i] == '-':
			lexems <- Lexem{MINUS, "-"}
		case runes[i] == '*':
			lexems <- Lexem{MUL, "*"}
		case runes[i] == '/':
			lexems <- Lexem{DIV, "/"}
		case runes[i] == '(':
			lexems <- Lexem{LPAREN, "("}
		case runes[i] == ')':
			lexems <- Lexem{RPAREN, ")"}
		case unicode.IsSpace(runes[i]): // Пропуск пробелов
			continue
		default:
			lexems <- Lexem{ERROR, string(runes[i])}
		}
	}
	close(lexems)
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("error")
	} else {
		expr := os.Args[1]
		lexems := make(chan Lexem)
		go lexer(expr, lexems)
		parser := NewParser(lexems)
		result, ok := parser.E()
		if ok {
			fmt.Println(result)
		}
	}
}
