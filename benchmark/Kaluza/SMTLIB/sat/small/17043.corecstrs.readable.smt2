(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_118605 () String)

(assert (= T_1 (= "-" var_0xINPUT_118605)))
(assert T_1)

(check-sat)

