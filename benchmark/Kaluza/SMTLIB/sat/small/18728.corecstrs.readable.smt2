(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_17495 () String)

(assert (= T_1 (= "-" var_0xINPUT_17495)))
(assert T_1)

(check-sat)

