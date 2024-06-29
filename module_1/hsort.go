package main

func hsort(n int, less func(i, j int) bool, swap func(i, j int)) {

	var heapify func(int, int)
	heapify = func(n, i int) {
		largest := i
		left := 2*i + 1
		right := 2*i + 2

		if left < n && less(largest, left) {
			largest = left
		}

		if right < n && less(largest, right) {
			largest = right
		}

		if largest != i {
			swap(i, largest)
			heapify(n, largest)
		}
	}
	for i := n/2 - 1; i >= 0; i-- {
		heapify(n, i)
	}
	for i := n - 1; i > 0; i-- {
		swap(0, i)
		heapify(i, 0)
	}
}

func main() {

}
