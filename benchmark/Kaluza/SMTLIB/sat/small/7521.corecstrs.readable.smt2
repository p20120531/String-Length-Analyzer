(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_204155 () String)

(assert (= T_1 (not (= "pVEECEXk2p" var_0xINPUT_204155))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_204155 ""))))
(assert T_2)

(check-sat)

