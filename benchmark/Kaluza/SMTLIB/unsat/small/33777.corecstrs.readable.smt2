(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_79043 () String)

(assert (= T_1 (not (= "" var_0xINPUT_79043))))
(assert T_1)

(check-sat)

