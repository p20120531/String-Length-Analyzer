(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_84844 () String)

(assert (= T_1 (not (= "labpixiesdata.appspot.com" var_0xINPUT_84844))))
(assert T_1)

(check-sat)

