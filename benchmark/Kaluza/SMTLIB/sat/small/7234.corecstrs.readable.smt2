(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_174833 () String)

(assert (= T_1 (not (= "cm6AfB9De3" var_0xINPUT_174833))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_174833 "CQALcCP5TB"))))
(assert T_2)

(check-sat)

