(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_125347 () String)

(assert (= T_1 (= "-" var_0xINPUT_125347)))
(assert T_1)

(check-sat)

