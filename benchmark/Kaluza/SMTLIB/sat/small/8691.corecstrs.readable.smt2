(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_102826 () String)

(assert (= T_1 (= "bIs4392oYa" var_0xINPUT_102826)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

