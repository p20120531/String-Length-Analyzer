(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_549993 () String)

(assert (= T_1 (= "-" var_0xINPUT_549993)))
(assert T_1)

(check-sat)

