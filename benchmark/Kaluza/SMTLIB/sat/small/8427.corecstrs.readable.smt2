(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_101996 () String)

(assert (= T_1 (= "Xi1EY655MX" var_0xINPUT_101996)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

