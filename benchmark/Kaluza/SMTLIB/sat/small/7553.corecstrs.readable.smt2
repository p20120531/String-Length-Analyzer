(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_88398 () String)

(assert (= T_1 (not (= "AUEAICDdfN" var_0xINPUT_88398))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_88398 ""))))
(assert T_2)

(check-sat)

