(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_102882 () String)

(assert (= T_1 (= "egKU4SiReL" var_0xINPUT_102882)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

