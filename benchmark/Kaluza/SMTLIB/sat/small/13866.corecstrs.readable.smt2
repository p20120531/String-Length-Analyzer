(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_150441 () String)

(assert (= T_1 (= "https:" var_0xINPUT_150441)))
(assert T_1)

(check-sat)

