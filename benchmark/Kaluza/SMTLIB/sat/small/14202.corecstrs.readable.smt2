(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_276788 () String)

(assert (= T_1 (= var_0xINPUT_276788 "Search")))
(assert T_1)

(check-sat)

