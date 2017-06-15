(set-logic QF_S)
(set-option :produce-models true)


(declare-fun v1 () String)
(declare-fun v3 () String)



(assert (= v1 "aabac"))
(assert (= v3 (str.replace v1 "ab" "d")))
(assert (= 3 (str.len v3)))

(check-sat)
(get-model)