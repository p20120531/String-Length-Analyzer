(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_17576 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_17576))))
(assert T_1)

(check-sat)

