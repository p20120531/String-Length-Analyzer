(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun var_0xINPUT_104821 () String)

(assert (= T_1 (not (= "pVEECEXk2p" var_0xINPUT_104821))))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (not (= var_0xINPUT_104821 ""))))
(assert T_3)

(check-sat)

