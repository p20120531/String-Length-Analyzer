(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_14819 () String)

(assert (= T_1 (= "-" var_0xINPUT_14819)))
(assert T_1)

(check-sat)

