(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun var_0xINPUT_77071 () String)

(assert (= T_1 (not (= "CQALcCP5TB" var_0xINPUT_77071))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_77071 "AA1ND6MEXd"))))
(assert (= T_3 (not T_2)))
(assert T_3)

(check-sat)

