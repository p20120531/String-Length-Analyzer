(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_96068 () String)

(assert (= T_1 (= "-" var_0xINPUT_96068)))
(assert T_1)

(check-sat)

