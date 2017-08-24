(set-logic QF_S)


(declare-fun v1 () String)


(assert (= v1 "a"))
(assert (str.prefixof v1 "abc"))

(check-sat)
