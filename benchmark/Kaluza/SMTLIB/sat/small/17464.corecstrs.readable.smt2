(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_160868 () String)

(assert (= T_1 (= "-" var_0xINPUT_160868)))
(assert T_1)

(check-sat)

