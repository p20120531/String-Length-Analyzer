(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_118141 () String)

(assert (= T_1 (= "PEYGwndb6c" var_0xINPUT_118141)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

