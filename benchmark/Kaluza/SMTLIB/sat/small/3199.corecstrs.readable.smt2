(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)
(declare-fun T1_8 () String)
(declare-fun T2_8 () String)
(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_5 () String)
(declare-fun var_0xINPUT_224090 () String)

(assert (= T_1 (not (= "" var_0xINPUT_224090))))
(assert T_1)
(assert (= T_2 (= var_0xINPUT_224090 "array")))
(assert (= T_3 (not T_2)))
(assert T_3)
(assert (= PCTEMP_LHS_1 var_0xINPUT_224090))
(assert (not (str.in.re PCTEMP_LHS_1 (str.to.re "%"))))
(assert (= T_5 (str.++ T1_8 T2_8)))
(assert (= T2_8 PCTEMP_LHS_1))
(assert (= T1_8 "subscribe="))

(check-sat)

