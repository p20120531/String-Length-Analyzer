(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_8141 () String)

(assert (= T_1 (= var_0xINPUT_8141 "file:")))
(assert T_1)

(check-sat)

