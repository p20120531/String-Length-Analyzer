(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_150083 () String)

(assert (= T_1 (= "https:" var_0xINPUT_150083)))
(assert T_1)

(check-sat)

