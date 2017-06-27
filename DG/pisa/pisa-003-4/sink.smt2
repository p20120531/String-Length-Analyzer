(set-logic QF_S)
(set-option :produce-models true)

(declare-fun s () String)
(declare-fun var () String)
(declare-fun ret () String)



(assert (not (str.contains s "<") ) )
(assert (not (str.contains s ">") ) )
(assert (= ret s) )
(assert (= var (str.++ "<scr" "ipt") ) )
(assert (str.contains s var) )
(assert (not (= ret "x") ) )


(check-sat)
(get-model)
