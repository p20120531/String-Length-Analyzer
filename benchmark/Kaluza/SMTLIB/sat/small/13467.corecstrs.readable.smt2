(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_123867 () String)

(assert (= T_1 (= "D1aHYb9COU" var_0xINPUT_123867)))
(assert T_1)

(check-sat)
