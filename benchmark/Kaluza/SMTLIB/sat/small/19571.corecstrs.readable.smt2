(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_42487 () String)

(assert (= T_1 (not (= var_0xINPUT_42487 ""))))
(assert T_1)

(check-sat)

