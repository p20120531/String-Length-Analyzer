(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_124468 () String)

(assert (= T_1 (= "-" var_0xINPUT_124468)))
(assert T_1)

(check-sat)

