(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_98464 () String)

(assert (= T_1 (not (= "fB9De3KAY9" var_0xINPUT_98464))))
(assert T_1)

(check-sat)

