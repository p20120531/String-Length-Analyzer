(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_96362 () String)

(assert (= T_1 (not (= "L2e4ETpOua" var_0xINPUT_96362))))
(assert T_1)

(check-sat)

