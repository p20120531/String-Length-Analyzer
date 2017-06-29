(set-logic QF_S)
(set-option :produce-models true)

(declare-fun s () String)
(declare-fun var () String)



(assert (str.contains s ">") )
(assert (= var (str.++ "<scr" "ipt") ) )
(assert (str.contains s var) )


(check-sat)
(get-model)
