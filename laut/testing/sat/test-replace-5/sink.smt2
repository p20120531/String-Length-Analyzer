(set-logic QF_S)


(declare-fun v1 () String)
(declare-fun v3 () String)
(declare-fun i1 () Int)
(declare-fun i2 () Int)


(assert (str.in.re v1 (re.* (str.to.re "a"))))
(assert (= v3 (str.replace v1 "aa" "d")))
(assert (= i1 (str.len v3)))
(assert (= i2 (str.len v1)))
(assert (= 9 (+ i1 i2)))

(check-sat)
