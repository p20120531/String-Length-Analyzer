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
(declare-fun var_0xINPUT_264211 () String)

(assert (= T_1 (not (= "" var_0xINPUT_264211))))
(assert T_1)
(assert (= T_2 (not (= "" var_0xINPUT_264211))))
(assert T_2)
(assert (= T_3 (not (= "VZCCTTfYY8" var_0xINPUT_264211))))
(assert T_3)
(assert (= T_4 (not (= var_0xINPUT_264211 "VZCCTTfYY8"))))
(assert T_4)
(assert (= T_5 (= var_0xINPUT_264211 "")))
(assert (= T_6 (not T_5)))
(assert T_6)
(assert (= T_7 (not (= "" var_0xINPUT_264211))))
(assert T_7)
(assert (= T_8 (not (= var_0xINPUT_264211 "o7E1eVAEe3"))))
(assert (= T_9 (not T_8)))
(assert T_9)

(check-sat)

