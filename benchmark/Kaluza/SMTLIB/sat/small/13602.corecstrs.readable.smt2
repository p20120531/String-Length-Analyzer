(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_120359 () String)

(assert (= T_1 (not (= "o7E1eVAEe3" var_0xINPUT_120359))))
(assert T_1)

(check-sat)

