(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_102534 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_102534))))
(assert T_1)

(check-sat)

