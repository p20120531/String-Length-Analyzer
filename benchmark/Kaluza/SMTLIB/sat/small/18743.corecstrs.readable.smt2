(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_18441 () String)

(assert (= T_1 (= "-" var_0xINPUT_18441)))
(assert T_1)

(check-sat)

