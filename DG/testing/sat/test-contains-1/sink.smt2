(set-logic QF_S)


(declare-fun v1 () String)
(declare-fun i2 () Int)


(assert (= v1 "abcdef"))
(assert (str.contains v1 ""))
(assert (= i2 (str.len v1)))

(check-sat)
