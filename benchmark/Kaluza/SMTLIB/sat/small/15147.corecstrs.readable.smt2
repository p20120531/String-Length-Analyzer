(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_302866 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_302866))))
(assert T_1)

(check-sat)

