(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_65043 () String)

(assert (= T_1 (not (= "qXdHDo7IZR" var_0xINPUT_65043))))
(assert T_1)

(check-sat)

