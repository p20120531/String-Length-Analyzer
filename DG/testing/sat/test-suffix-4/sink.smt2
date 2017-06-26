(set-logic QF_S)


(declare-fun v1 () String)


(assert (= v1 "abc"))
(assert (= 3 (str.len v1)))
(assert (str.suffixof v1 "abc"))

(check-sat)
