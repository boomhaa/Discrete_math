package main

import (
	"fmt"
)

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func abs(a int) int {
	if a < 0 {
		return -a
	}
	return a
}

func gcd(a, b int) int {
	a, b = abs(a), abs(b)
	for a != 0 && b != 0 {
		if a > b {
			a %= b
		} else {
			b %= a
		}
	}
	return max(a, b)
}

func swap(matrix [][]drob, i, j, n int) [][]drob {
	for k := 0; k < n+1; k++ {
		matrix[i][k], matrix[j][k] = matrix[j][k], matrix[i][k]
	}
	return matrix
}

type drob struct {
	chisl, znam int
}

func solution(matrix [][]drob, n int) ([]drob, bool) {
	var solve []drob

	for i := 0; i < n-1; i++ {

		if matrix[i][i].chisl == 0 {
			is := true
			for j := i + 1; j < n; j++ {
				if matrix[j][i].chisl != 0 {
					is = false
					matrix = swap(matrix, i, j, n)
					break
				}
			}
			if is {
				for i := 0; i < n; i++ {
					for j := 0; j < n+1; j++ {
						fmt.Print(matrix[i][j].chisl, "/", matrix[i][j].znam, " ")
					}
					fmt.Println()
				}
				return solve, false
			}
		}

		for j := i + 1; j < n; j++ {
			var new_drob drob
			new_drob.chisl = matrix[j][i].chisl * matrix[i][i].znam
			new_drob.znam = matrix[j][i].znam * matrix[i][i].chisl
			for k := i; k < n+1; k++ {
				matrix[j][k].chisl = matrix[j][k].chisl*matrix[j-1][k].znam*new_drob.znam - matrix[j][k].znam*matrix[j-1][k].chisl*new_drob.chisl
				matrix[j][k].znam = matrix[j][k].znam * matrix[j-1][k].znam * new_drob.znam
				if matrix[j][k].chisl == 0 {
					matrix[j][k].znam = 0
				} else {
					gcd_a_b := gcd(matrix[j][k].chisl, matrix[j][k].znam)
					matrix[j][k].chisl /= gcd_a_b
					matrix[j][k].znam /= gcd_a_b
				}
				if matrix[j][k].znam < 0 {
					matrix[j][k].znam = abs(matrix[j][k].znam)
					matrix[j][k].chisl = -matrix[j][k].chisl
				}
			}
		}
	}
	for i := 0; i < n; i++ {
		for j := 0; j < n+1; j++ {
			fmt.Print(matrix[i][j].chisl, "/", matrix[i][j].znam, " ")
		}
		fmt.Println()
	}
	return solve, true
}

func main() {
	var n int
	fmt.Scan(&n)
	var matrix [][]drob
	for i := 0; i < n; i++ {
		matrix = append(matrix, []drob{})
		for j := 0; j < n+1; j++ {
			var value int
			fmt.Scan(&value)
			var new_drob drob
			new_drob.chisl = value
			if value == 0 {
				new_drob.znam = 0
			} else {
				new_drob.znam = 1
			}
			matrix[i] = append(matrix[i], new_drob)
		}
	}
	sol, have_sol := solution(matrix, n)
	if have_sol {
		fmt.Print(sol)
	} else {
		fmt.Println("No solution")
	}
}
