(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_8250 () String)

(assert (= T_1 (not (= "" var_0xINPUT_8250))))
(assert T_1)

(check-sat)

