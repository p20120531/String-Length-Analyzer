(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_147990 () String)

(assert (= T_1 (= "TebcMJKr8R" var_0xINPUT_147990)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

