(set-logic QF_S)

(declare-fun T1_3 () String)
(declare-fun T2_3 () String)
(declare-fun T_1 () String)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_36653 () String)

(assert (= T_1 (str.++ T1_3 T2_3)))
(assert (= T2_3 var_0xINPUT_36653))
(assert (= T1_3 ""))
(assert (= T_2 (= "" T_1)))
(assert T_2)

(check-sat)

