(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun var_0xINPUT_110538 () String)

(assert (= T_1 (not (= "AA1ND6MEXd" var_0xINPUT_110538))))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (not (= var_0xINPUT_110538 ""))))
(assert T_3)

(check-sat)

