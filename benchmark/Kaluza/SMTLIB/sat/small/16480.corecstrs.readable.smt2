(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_95110 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_95110))))
(assert T_1)

(check-sat)

