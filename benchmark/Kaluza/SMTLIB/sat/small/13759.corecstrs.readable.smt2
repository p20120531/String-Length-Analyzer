(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_87324 () String)

(assert (= T_1 (not (= "pVEECEXk2p" var_0xINPUT_87324))))
(assert T_1)

(check-sat)

