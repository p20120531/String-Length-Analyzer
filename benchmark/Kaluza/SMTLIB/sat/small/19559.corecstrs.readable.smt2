(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_37171 () String)

(assert (= T_1 (not (= var_0xINPUT_37171 ""))))
(assert T_1)

(check-sat)

