(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_102935 () String)

(assert (= T_1 (= "iDBGE5i1D5" var_0xINPUT_102935)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

