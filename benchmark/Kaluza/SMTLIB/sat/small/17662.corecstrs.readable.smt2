(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_102207 () String)

(assert (= T_1 (= "-" var_0xINPUT_102207)))
(assert T_1)

(check-sat)

