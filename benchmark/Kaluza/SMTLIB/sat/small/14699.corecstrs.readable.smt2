(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_109111 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_109111))))
(assert T_1)

(check-sat)

