(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_311436 () String)

(assert (= T_1 (= var_0xINPUT_311436 "Search")))
(assert T_1)

(check-sat)

