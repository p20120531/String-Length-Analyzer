(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_112961 () String)

(assert (= T_1 (= "-" var_0xINPUT_112961)))
(assert T_1)

(check-sat)

