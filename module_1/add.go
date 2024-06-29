package main

func max(a, b int) int {
	if a < b {
		return b
	}
	return a
}
func min(a, b int) int {
	if a > b {
		return b
	}
	return a
}

func add(a, b []int32, p int) []int32 {
	res := make([]int32, 0, max(len(a), len(b)))
	var q int32
	for i := 0; i < max(len(a), len(b)); i++ {
		if i >= len(a) {
			res = append(res, (b[i]+q)%int32(p))
			q = (b[i] + q) / int32(p)
		} else if i >= len(b) {
			res = append(res, (a[i]+q)%int32(p))
			q = (a[i] + q) / int32(p)
		} else {
			res = append(res, (a[i]+b[i]+q)%int32(p))
			q = (a[i] + b[i] + q) / int32(p)
		}

	}
	if q > 0 {
		res = append(res, q)
	}
	return res
}

func main() {

}
