(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_97067 () String)

(assert (= T_1 (not (= "VEECEXk2pV" var_0xINPUT_97067))))
(assert T_1)

(check-sat)

