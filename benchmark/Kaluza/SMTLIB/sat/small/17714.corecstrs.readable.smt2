(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_125191 () String)

(assert (= T_1 (= "-" var_0xINPUT_125191)))
(assert T_1)

(check-sat)

