(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_112682 () String)

(assert (= T_1 (= "-" var_0xINPUT_112682)))
(assert T_1)

(check-sat)

