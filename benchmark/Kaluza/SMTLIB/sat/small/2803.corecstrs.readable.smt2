(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_b () Bool)
(declare-fun var_0xINPUT_120636 () String)

(assert (= T_1 (not (= "o7E1eVAEe3" var_0xINPUT_120636))))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (not (= "" var_0xINPUT_120636))))
(assert T_3)
(assert (= T_4 (not (= var_0xINPUT_120636 ""))))
(assert T_4)
(assert (= T_5 (not (= "" var_0xINPUT_120636))))
(assert T_5)
(assert (= T_6 (= var_0xINPUT_120636 "")))
(assert (= T_7 (not T_6)))
(assert T_7)
(assert (= T_8 (not (= "" var_0xINPUT_120636))))
(assert T_8)
(assert (= T_9 (= var_0xINPUT_120636 "Example:")))
(assert (= T_a (not T_9)))
(assert T_a)
(assert (= T_b (not (= var_0xINPUT_120636 "VZCCTTfYY8"))))
(assert T_b)

(check-sat)

