(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_55307 () String)

(assert (= T_1 (not (= "4PH2ujcENG" var_0xINPUT_55307))))
(assert T_1)

(check-sat)

