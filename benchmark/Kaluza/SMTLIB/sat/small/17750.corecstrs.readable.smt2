(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_313116 () String)

(assert (= T_1 (= "-" var_0xINPUT_313116)))
(assert T_1)

(check-sat)

