(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_468893 () String)

(assert (= T_1 (= var_0xINPUT_468893 "Search")))
(assert T_1)

(check-sat)

