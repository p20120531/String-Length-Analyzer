(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)
(declare-fun T1_2 () String)
(declare-fun T1_6 () String)
(declare-fun T2_2 () String)
(declare-fun T2_6 () String)
(declare-fun T_2 () String)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun var_0xINPUT_2 () String)

(assert (= PCTEMP_LHS_1 (str.++ T1_2 T2_2)))
(assert (= T2_2 "=Online"))
(assert (= T1_2 var_0xINPUT_2))
(assert (= T_2 (str.++ T1_6 T2_6)))
(assert (= T2_6 "Now"))
(assert (= T1_6 PCTEMP_LHS_1))
(assert (= T_3 (not (= T_2 "Hello=Joe=OnlineNow"))))
(assert (= T_4 (not T_3)))
(assert T_4)

(check-sat)

