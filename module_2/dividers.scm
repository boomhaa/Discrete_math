(define (get-divisors x divisorsl divisorsr i)
  (if (> (* i i) x)
      (append divisorsr divisorsl)
      (if (= (modulo x i) 0)
          (if (= (/ x i) i)
              (get-divisors x (cons i divisorsl) divisorsr (+ i 1))
              (get-divisors x (cons i divisorsl) (append divisorsr (list (/ x i))) (+ i 1)))
          (get-divisors x divisorsl divisorsr (+ i 1)))))

(define (between a b)
  (let loop ((i 2))
    (if (<= i (sqrt (quotient a b) ))
        (if (= (modulo (quotient a b) i) 0)
            #f
            (loop (+ i 1)))
        #t)))

(define (display-graph dividers len)
  (begin
    (display "graph {\n")
    (let loop ((i 0))
      (if (< i len)
          (begin
            (display (string-append "\t" (number->string (list-ref dividers i)) "\n"))
            (loop (+ i 1)))))
    (let loop ((i 0))
      (if (< i len)
          (begin
            (let loop2 ((j (+ i 1)))
              (if (< j len)
                  (begin
                    (if (and (= (modulo (list-ref dividers i) (list-ref dividers j)) 0)
                             (between (list-ref dividers i) (list-ref dividers j)))
                        (display (string-append "\t" (number->string (list-ref dividers i)) "--"
                                                (number->string (list-ref dividers j)) "\n")))
                    (loop2 (+ j 1)))))
            (loop (+ i 1)))))
    (display "}")))

(define n (read))
(define dividers (get-divisors n '() '() 1))
(display-graph dividers (length dividers))