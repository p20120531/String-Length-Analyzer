(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_67480 () String)

(assert (= T_1 (not (= "ND6MEXdqBD" var_0xINPUT_67480))))
(assert T_1)

(check-sat)

