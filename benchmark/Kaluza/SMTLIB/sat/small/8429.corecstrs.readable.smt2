(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_102054 () String)

(assert (= T_1 (= "3wbCdwGSRC" var_0xINPUT_102054)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

