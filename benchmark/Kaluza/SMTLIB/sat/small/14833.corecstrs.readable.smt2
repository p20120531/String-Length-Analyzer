(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_101973 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_101973))))
(assert T_1)

(check-sat)

