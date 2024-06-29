package main

func encode(utf32 []rune) []byte {
	var utf8Bytes []byte

	for _, codePoint := range utf32 {
		if codePoint <= 0x7F {
			utf8Bytes = append(utf8Bytes, byte(codePoint))
		} else if codePoint <= 0x7FF {
			utf8Bytes = append(utf8Bytes, byte(0xC0|(codePoint>>6)))
			utf8Bytes = append(utf8Bytes, byte(0x80|(codePoint&0x3F)))
		} else if codePoint <= 0xFFFF {
			utf8Bytes = append(utf8Bytes, byte(0xE0|(codePoint>>12)))
			utf8Bytes = append(utf8Bytes, byte(0x80|((codePoint>>6)&0x3F)))
			utf8Bytes = append(utf8Bytes, byte(0x80|(codePoint&0x3F)))
		} else {
			utf8Bytes = append(utf8Bytes, byte(0xF0|(codePoint>>18)))
			utf8Bytes = append(utf8Bytes, byte(0x80|((codePoint>>12)&0x3F)))
			utf8Bytes = append(utf8Bytes, byte(0x80|((codePoint>>6)&0x3F)))
			utf8Bytes = append(utf8Bytes, byte(0x80|(codePoint&0x3F)))
		}
	}

	return utf8Bytes
}

func decode(utf8 []byte) []rune {
	var utf32 []rune

	for i := 0; i < len(utf8); {
		if utf8[i]&0x80 == 0 {
			utf32 = append(utf32, rune(utf8[i]))
			i++
		} else if utf8[i]>>5 == 0x6 {
			codePoint := (rune(utf8[i]&0x1F) << 6) | rune(utf8[i+1]&0x3F)
			utf32 = append(utf32, codePoint)
			i += 2
		} else if utf8[i]>>4 == 0xE {
			codePoint := (rune(utf8[i]&0xF) << 12) | (rune(utf8[i+1]&0x3F) << 6) | rune(utf8[i+2]&0x3F)
			utf32 = append(utf32, codePoint)
			i += 3
		} else if utf8[i]>>3 == 0x1E {
			codePoint := (rune(utf8[i]&0x7) << 18) | (rune(utf8[i+1]&0x3F) << 12) | (rune(utf8[i+2]&0x3F) << 6) | rune(utf8[i+3]&0x3F)
			utf32 = append(utf32, codePoint)
			i += 4
		}
	}

	return utf32
}

func main() {

}
