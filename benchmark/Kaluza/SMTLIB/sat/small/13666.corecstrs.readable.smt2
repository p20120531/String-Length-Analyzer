(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_168681 () String)

(assert (= T_1 (not (= "VZCCTTfYY8" var_0xINPUT_168681))))
(assert T_1)

(check-sat)

