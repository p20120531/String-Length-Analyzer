(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_230338 () String)

(assert (= T_1 (not (= "3s71lxYG6e" var_0xINPUT_230338))))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

