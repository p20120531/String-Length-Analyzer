(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_105968 () String)

(assert (= T_1 (= "Rji1FP3tvB" var_0xINPUT_105968)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

