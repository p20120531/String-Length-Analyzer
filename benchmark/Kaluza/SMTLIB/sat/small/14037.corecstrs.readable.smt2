(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_390451 () String)

(assert (= T_1 (= var_0xINPUT_390451 "Search")))
(assert T_1)

(check-sat)

