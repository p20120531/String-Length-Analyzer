(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_26880 () String)

(assert (= T_1 (= "-" var_0xINPUT_26880)))
(assert T_1)

(check-sat)

