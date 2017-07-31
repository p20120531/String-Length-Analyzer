(set-logic QF_S)

(declare-fun I0_11 () Int)
(declare-fun I0_24 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_11 () String)
(declare-fun T0_24 () String)
(declare-fun T1_10 () String)
(declare-fun T1_11 () String)
(declare-fun T1_17 () String)
(declare-fun T1_20 () String)
(declare-fun T1_23 () String)
(declare-fun T1_24 () String)
(declare-fun T1_4 () String)
(declare-fun T1_7 () String)
(declare-fun T2_10 () String)
(declare-fun T2_11 () String)
(declare-fun T2_17 () String)
(declare-fun T2_20 () String)
(declare-fun T2_23 () String)
(declare-fun T2_24 () String)
(declare-fun T2_4 () String)
(declare-fun T2_7 () String)
(declare-fun T3_11 () String)
(declare-fun T3_24 () String)
(declare-fun T4_11 () String)
(declare-fun T4_24 () String)
(declare-fun T5_11 () String)
(declare-fun T5_24 () String)
(declare-fun T_1 () String)
(declare-fun T_11 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () String)
(declare-fun T_5 () Bool)
(declare-fun T_6 () String)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_a () String)
(declare-fun T_b () Bool)
(declare-fun T_c () Bool)
(declare-fun T_d () String)
(declare-fun T_e () Bool)
(declare-fun T_f () String)
(declare-fun var_0xINPUT_136698 () String)

(assert (= T_1 (str.++ T1_4 T2_4)))
(assert (= T2_4 var_0xINPUT_136698))
(assert (= T1_4 ""))
(assert (= T_2 (= "-" T_1)))
(assert (= T_3 (not T_2)))
(assert T_3)
(assert (= T_4 (str.++ T1_7 T2_7)))
(assert (= T2_7 var_0xINPUT_136698))
(assert (= T1_7 ""))
(assert (= T_5 (= "" T_4)))
(assert T_5)
(assert (= T_6 (str.++ T1_10 T2_10)))
(assert (= T2_10 var_0xINPUT_136698))
(assert (= T1_10 ""))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_11 0))(= T_6 (str.++ T0_11 T1_11))(= I0_11 (str.len T4_11))(= 0 (str.len T0_11))(= T1_11 (str.++ T2_11 T3_11))(= T2_11 (str.++ T4_11 T5_11))(= T5_11 "utm_id=")(not (str.in.re T4_11 (re.++ (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "_") (str.to.re "i") (str.to.re "d") (str.to.re "="))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= T_6 (str.++ T0_11 T1_11))(= 0 (str.len T0_11))(not (str.in.re T1_11 (re.++ (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "_") (str.to.re "i") (str.to.re "d") (str.to.re "=")))))))
(assert (= T_8 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_9 (not T_8)))
(assert T_9)
(assert (= T_a (str.++ T1_17 T2_17)))
(assert (= T2_17 var_0xINPUT_136698))
(assert (= T1_17 ""))
(assert (= T_b (= "-" T_a)))
(assert (= T_c (not T_b)))
(assert T_c)
(assert (= T_d (str.++ T1_20 T2_20)))
(assert (= T2_20 var_0xINPUT_136698))
(assert (= T1_20 ""))
(assert (= T_e (= "" T_d)))
(assert T_e)
(assert (= T_f (str.++ T1_23 T2_23)))
(assert (= T2_23 var_0xINPUT_136698))
(assert (= T1_23 ""))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_24 0))(= T_f (str.++ T0_24 T1_24))(= I0_24 (str.len T4_24))(= 0 (str.len T0_24))(= T1_24 (str.++ T2_24 T3_24))(= T2_24 (str.++ T4_24 T5_24))(= T5_24 "src=")(not (str.in.re T4_24 (re.++ (str.to.re "s") (str.to.re "r") (str.to.re "c") (str.to.re "="))))) 
	(and (= PCTEMP_LHS_2 (- 1))(= T_f (str.++ T0_24 T1_24))(= 0 (str.len T0_24))(not (str.in.re T1_24 (re.++ (str.to.re "s") (str.to.re "r") (str.to.re "c") (str.to.re "=")))))))
(assert (= T_11 (< (- 1) PCTEMP_LHS_2)))
(assert T_11)

(check-sat)

