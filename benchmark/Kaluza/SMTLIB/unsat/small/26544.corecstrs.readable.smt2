(set-logic QF_S)

(declare-fun T1_8 () String)
(declare-fun T2_8 () String)
(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () String)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun var_0xINPUT_15313 () String)
(declare-fun var_0xINPUT_47800 () String)

(assert (= T_1 (not (= "" var_0xINPUT_15313))))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (not (= "" var_0xINPUT_15313))))
(assert T_3)
(assert (= T_4 (str.++ T1_8 T2_8)))
(assert (= T2_8 var_0xINPUT_47800))
(assert (= T1_8 ""))
(assert (= T_5 (= "" T_4)))
(assert (= T_6 (not T_5)))
(assert T_6)

(check-sat)

