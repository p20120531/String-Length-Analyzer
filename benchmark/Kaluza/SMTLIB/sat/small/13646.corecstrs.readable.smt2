(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_154418 () String)

(assert (= T_1 (not (= "CQALcCP5TB" var_0xINPUT_154418))))
(assert T_1)

(check-sat)

