(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_87518 () String)

(assert (= T_1 (not (= "AA1ND6MEXd" var_0xINPUT_87518))))
(assert T_1)

(check-sat)

