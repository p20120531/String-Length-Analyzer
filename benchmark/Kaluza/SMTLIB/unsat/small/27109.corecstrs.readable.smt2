(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun var_0xINPUT_14539 () String)

(assert (= T_1 (not (= "" var_0xINPUT_14539))))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (not (= "" var_0xINPUT_14539))))
(assert (= T_4 (not T_3)))
(assert T_4)
(assert (= T_5 (not (= "" var_0xINPUT_14539))))
(assert (= T_6 (not T_5)))
(assert T_6)

(check-sat)

