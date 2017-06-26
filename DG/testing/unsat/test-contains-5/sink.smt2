(set-logic QF_S)


(declare-fun v1 () String)
(declare-fun i1 () Int)
(declare-fun v2 () String)

(assert (= v1 "ab"))
(assert (str.contains v1 v2))
(assert (= i1 (str.len v1)))
(assert (str.in.re v2 (re.+ (str.to.re "bb"))))

(check-sat)
