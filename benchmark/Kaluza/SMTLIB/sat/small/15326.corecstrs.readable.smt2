(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_256903 () String)

(assert (= T_1 (= var_0xINPUT_256903 "file:")))
(assert T_1)

(check-sat)

