(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_89849 () String)

(assert (= T_1 (= "-" var_0xINPUT_89849)))
(assert T_1)

(check-sat)

