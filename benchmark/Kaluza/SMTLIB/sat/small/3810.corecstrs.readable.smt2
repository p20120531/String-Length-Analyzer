(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun var_0xINPUT_93942 () String)

(assert (= T_1 (not (= "cm6AfB9De3" var_0xINPUT_93942))))
(assert T_1)
(assert (= T_2 (not (= "" var_0xINPUT_93942))))
(assert T_2)
(assert (= T_3 (not (= var_0xINPUT_93942 "ywUN4ZIDAC"))))
(assert (= T_4 (not T_3)))
(assert T_4)

(check-sat)

