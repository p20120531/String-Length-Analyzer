(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_4903 () String)

(assert (= T_1 (not (= "" var_0xINPUT_4903))))
(assert T_1)

(check-sat)

