(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_102854 () String)

(assert (= T_1 (= "TeQEA0JCLg" var_0xINPUT_102854)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

