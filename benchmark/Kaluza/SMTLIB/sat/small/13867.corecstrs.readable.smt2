(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_150480 () String)

(assert (= T_1 (= "https:" var_0xINPUT_150480)))
(assert T_1)

(check-sat)

