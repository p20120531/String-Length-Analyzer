(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_163734 () String)

(assert (= T_1 (not (= "CQALcCP5TB" var_0xINPUT_163734))))
(assert T_1)

(check-sat)

