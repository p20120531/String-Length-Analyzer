(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_107358 () String)

(assert (= T_1 (= "-" var_0xINPUT_107358)))
(assert T_1)

(check-sat)

