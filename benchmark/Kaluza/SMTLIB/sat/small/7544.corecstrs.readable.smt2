(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_53172 () String)

(assert (= T_1 (not (= "ywUN4ZIDAC" var_0xINPUT_53172))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_53172 ""))))
(assert T_2)

(check-sat)

