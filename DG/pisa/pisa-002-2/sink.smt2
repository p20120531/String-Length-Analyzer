(set-logic QF_S)
(set-option :produce-models true)

(declare-fun ret () String)


(assert (= ret "x" ) )
(assert (not (= ret "x") ) )


(check-sat)
(get-model)
