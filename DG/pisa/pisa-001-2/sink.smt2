(set-logic QF_S)
(set-option :produce-models true)


(declare-fun v1 () String)
(declare-fun ret () String)



(assert (not (str.contains v1 "<") ) )
(assert (= ret v1) )



(check-sat)
(get-model)
