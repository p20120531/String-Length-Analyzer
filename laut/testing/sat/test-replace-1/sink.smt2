(set-logic QF_S)


(declare-fun v1 () String)
(declare-fun v3 () String)



(assert (= v1 "aabac"))
(assert (= v3 (str.replace v1 "ab" "d")))


(check-sat)
