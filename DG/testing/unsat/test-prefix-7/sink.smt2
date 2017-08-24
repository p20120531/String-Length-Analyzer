(set-logic QF_S)


(declare-fun v1 () String)
(declare-fun v2 () String)
(declare-fun i1 () Int)

(assert (str.in.re v1 (re.* (str.to.re "a"))))
(assert (= i1 (str.len v1)))
(assert (> i1 2))
(assert (str.prefixof v1 v2))
(assert (str.in.re v2 (re.union (str.to.re "aa") (str.to.re "a"))))

(check-sat)
