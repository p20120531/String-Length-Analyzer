(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_91674 () String)

(assert (= T_1 (not (= "3s71lxYG6e" var_0xINPUT_91674))))
(assert T_1)

(check-sat)

