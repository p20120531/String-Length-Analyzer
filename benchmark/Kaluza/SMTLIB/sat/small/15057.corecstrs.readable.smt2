(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_57748 () String)

(assert (= T_1 (= var_0xINPUT_57748 "Search")))
(assert T_1)

(check-sat)

