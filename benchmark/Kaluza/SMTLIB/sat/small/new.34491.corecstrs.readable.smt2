(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_17303 () String)

(assert (= T_1 (not (= var_0xINPUT_17303 ""))))
(assert T_1)

(check-sat)

