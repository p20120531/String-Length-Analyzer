(set-logic QF_S)


(declare-fun v1 () String)


(assert (= v1 ""))
(assert (str.suffixof v1 "abc"))

(check-sat)
