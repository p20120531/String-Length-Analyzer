(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_181437 () String)

(assert (= T_1 (not (= "K9BOAGJDFi" var_0xINPUT_181437))))
(assert T_1)

(check-sat)

