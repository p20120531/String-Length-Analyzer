(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_100735 () String)

(assert (= T_1 (= "-" var_0xINPUT_100735)))
(assert T_1)

(check-sat)

