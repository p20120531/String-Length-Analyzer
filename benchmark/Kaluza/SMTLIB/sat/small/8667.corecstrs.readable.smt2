(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_186259 () String)

(assert (= T_1 (= "35vd8oeY2x" var_0xINPUT_186259)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

