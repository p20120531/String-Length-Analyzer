(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_167727 () String)

(assert (= T_1 (= "SCb600QdSr" var_0xINPUT_167727)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

