(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_109062 () String)

(assert (= T_1 (= "-" var_0xINPUT_109062)))
(assert T_1)

(check-sat)

