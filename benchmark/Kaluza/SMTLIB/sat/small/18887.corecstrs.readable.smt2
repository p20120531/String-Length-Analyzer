(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_23618 () String)

(assert (= T_1 (= "-" var_0xINPUT_23618)))
(assert T_1)

(check-sat)

