(set-logic QF_S)
(declare-fun v1 () String)
(declare-fun v3 () Int)
(declare-fun ret () String)
(assert (str.contains v1 "<") )
;(assert (= v3 (str.indexof v1 "<" 0) ) )
(assert (not (str.contains v1 "<") ) ) ; for indexof
(assert (= v3 (- 0 1) ) ) ; for indexof
(assert (= ret (str.substr v1 0 v3) ) )
(assert (str.contains ret ">") )
(check-sat)
