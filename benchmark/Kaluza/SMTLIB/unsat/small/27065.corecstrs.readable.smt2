(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun var_0xINPUT_5015 () String)

(assert (= T_1 (= "" var_0xINPUT_5015)))
(assert T_1)
(assert (= T_2 (= "" var_0xINPUT_5015)))
(assert T_2)
(assert (= T_3 (= "" var_0xINPUT_5015)))
(assert T_3)
(assert (= T_4 (= "" var_0xINPUT_5015)))
(assert (= T_5 (not T_4)))
(assert T_5)

(check-sat)

