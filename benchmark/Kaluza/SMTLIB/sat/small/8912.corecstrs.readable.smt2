(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_206949 () String)

(assert (= T_1 (not (= "qXdHDo7IZR" var_0xINPUT_206949))))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

