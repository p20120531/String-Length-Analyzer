(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun var_0xINPUT_91508 () String)

(assert (= T_1 (not (= "GYFic2vbcJ" var_0xINPUT_91508))))
(assert T_1)
(assert (= T_2 (not (= "" var_0xINPUT_91508))))
(assert T_2)
(assert (= T_3 (= var_0xINPUT_91508 "Example:")))
(assert (= T_4 (not T_3)))
(assert T_4)
(assert (= T_5 (not (= var_0xINPUT_91508 "ywUN4ZIDAC"))))
(assert (= T_6 (not T_5)))
(assert T_6)

(check-sat)

