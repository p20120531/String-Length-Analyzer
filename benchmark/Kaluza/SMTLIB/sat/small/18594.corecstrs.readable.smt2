(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_24025 () String)

(assert (= T_1 (= "-" var_0xINPUT_24025)))
(assert T_1)

(check-sat)

