(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_106337 () String)

(assert (= T_1 (not (= "o7E1eVAEe3" var_0xINPUT_106337))))
(assert T_1)

(check-sat)

