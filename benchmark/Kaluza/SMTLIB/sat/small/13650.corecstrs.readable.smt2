(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_156383 () String)

(assert (= T_1 (not (= "qXdHDo7IZR" var_0xINPUT_156383))))
(assert T_1)

(check-sat)
