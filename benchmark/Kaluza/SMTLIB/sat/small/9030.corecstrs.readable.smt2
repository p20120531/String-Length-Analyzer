(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_79196 () String)

(assert (= T_1 (not (= "labpixiesdata.appspot.com" var_0xINPUT_79196))))
(assert T_1)

(check-sat)

