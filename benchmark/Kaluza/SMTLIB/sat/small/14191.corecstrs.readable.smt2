(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_198791 () String)

(assert (= T_1 (= var_0xINPUT_198791 "Search")))
(assert T_1)

(check-sat)

