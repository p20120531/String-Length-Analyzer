(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_104761 () String)

(assert (= T_1 (not (= "pVEECEXk2p" var_0xINPUT_104761))))
(assert T_1)

(check-sat)

