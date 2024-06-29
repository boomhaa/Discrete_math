package main

func MergeSeqSort(nitems int, compare func(i, j int) int, indices chan int) {
	chans := make([]chan int, nitems)
	for i := 0; i < nitems; i++ {
		chan1 := make(chan int)
		go func(i int) {
			chan1 <- i
			close(chan1)
		}(i)
		chans[i] = chan1
	}

	MergeChans := func(l chan int, r chan int) chan int {
		res := make(chan int)
		go func() {
			defer close(res)
			a, b := <-l
			c, d := <-r
			for b || d {
				if !b {
					res <- c
					c, d = <-r
				} else if !d {
					res <- a
					a, b = <-l
				} else {
					if compare(a, c) > 0 {
						res <- c
						c, d = <-r
					} else if compare(a, c) < 0 {
						res <- a
						a, b = <-l
					} else {
						if a < c {
							res <- a
							a, b = <-l
						} else {
							res <- c
							c, d = <-r
						}
					}
				}
			}
		}()
		return res
	}

	for len(chans) > 1 {
		newchans := []chan int{}
		for i := 0; i < len(chans)-(len(chans)%2); i += 2 {
			new_chan := MergeChans(chans[i], chans[i+1])
			newchans = append(newchans, new_chan)
		}
		if len(chans)%2 != 0 {
			newchans = append(newchans, chans[len(chans)-1])
		}
		chans = newchans

	}
	for i := range chans[0] {
		indices <- i
	}
	close(indices)
}

func main() {

}
