(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_35316 () String)

(assert (= T_1 (= "-" var_0xINPUT_35316)))
(assert T_1)

(check-sat)

