(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_41911 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_41911))))
(assert T_1)

(check-sat)

