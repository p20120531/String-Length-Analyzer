(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun var_0xINPUT_289995 () String)

(assert (= T_1 (not (= "ywUN4ZIDAC" var_0xINPUT_289995))))
(assert T_1)
(assert (= T_2 (not (= "" var_0xINPUT_289995))))
(assert T_2)
(assert (= T_3 (not (= "3s71lxYG6e" var_0xINPUT_289995))))
(assert T_3)

(check-sat)
