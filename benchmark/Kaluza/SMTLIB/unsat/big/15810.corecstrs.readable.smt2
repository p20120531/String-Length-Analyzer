(set-logic QF_S)

(declare-fun I0_13 () Int)
(declare-fun I0_20 () Int)
(declare-fun I0_6 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun PCTEMP_LHS_3 () Int)
(declare-fun T0_13 () String)
(declare-fun T0_20 () String)
(declare-fun T0_6 () String)
(declare-fun T1_13 () String)
(declare-fun T1_20 () String)
(declare-fun T1_6 () String)
(declare-fun T2_13 () String)
(declare-fun T2_20 () String)
(declare-fun T2_6 () String)
(declare-fun T3_13 () String)
(declare-fun T3_20 () String)
(declare-fun T3_6 () String)
(declare-fun T4_13 () String)
(declare-fun T4_20 () String)
(declare-fun T4_6 () String)
(declare-fun T5_13 () String)
(declare-fun T5_20 () String)
(declare-fun T5_6 () String)
(declare-fun T_1 () Bool)
(declare-fun T_10 () Bool)
(declare-fun T_12 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_SELECT_3 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_c () Bool)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun var_0xINPUT_119253 () String)

(assert (= T_1 (not (= "" var_0xINPUT_119253))))
(assert T_1)
(assert (= T_2 (= "-" var_0xINPUT_119253)))
(assert (= T_3 (not T_2)))
(assert T_3)
(assert (= T_4 (= "" var_0xINPUT_119253)))
(assert T_4)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_6 0))(= var_0xINPUT_119253 (str.++ T0_6 T1_6))(= I0_6 (str.len T4_6))(= 0 (str.len T0_6))(= T1_6 (str.++ T2_6 T3_6))(= T2_6 (str.++ T4_6 T5_6))(= T5_6 "__utma=16886264.")(not (str.in.re T4_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_119253 (str.++ T0_6 T1_6))(= 0 (str.len T0_6))(not (str.in.re T1_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_7 (not T_6)))
(assert T_7)
(assert (= T_8 (= "-" var_0xINPUT_119253)))
(assert (= T_9 (not T_8)))
(assert T_9)
(assert (= T_a (= "" var_0xINPUT_119253)))
(assert T_a)
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_13 0))(= var_0xINPUT_119253 (str.++ T0_13 T1_13))(= I0_13 (str.len T4_13))(= 0 (str.len T0_13))(= T1_13 (str.++ T2_13 T3_13))(= T2_13 (str.++ T4_13 T5_13))(= T5_13 "__utmb=16886264")(not (str.in.re T4_13 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "b") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4"))))) 
	(and (= PCTEMP_LHS_2 (- 1))(= var_0xINPUT_119253 (str.++ T0_13 T1_13))(= 0 (str.len T0_13))(not (str.in.re T1_13 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "b") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4")))))))
(assert (= T_c (< (- 1) PCTEMP_LHS_2)))
(assert (= T_d (not T_c)))
(assert T_d)
(assert (= T_e (= "-" var_0xINPUT_119253)))
(assert (= T_f (not T_e)))
(assert T_f)
(assert (= T_10 (= "" var_0xINPUT_119253)))
(assert T_10)
(assert (= T_SELECT_3 (not (= PCTEMP_LHS_3 (- 1)))))
(assert (ite T_SELECT_3 
	(and (= PCTEMP_LHS_3 (+ I0_20 0))(= var_0xINPUT_119253 (str.++ T0_20 T1_20))(= I0_20 (str.len T4_20))(= 0 (str.len T0_20))(= T1_20 (str.++ T2_20 T3_20))(= T2_20 (str.++ T4_20 T5_20))(= T5_20 "__utmc=16886264")(not (str.in.re T4_20 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "c") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4"))))) 
	(and (= PCTEMP_LHS_3 (- 1))(= var_0xINPUT_119253 (str.++ T0_20 T1_20))(= 0 (str.len T0_20))(not (str.in.re T1_20 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "c") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4")))))))
(assert (= T_12 (< (- 1) PCTEMP_LHS_3)))
(assert T_12)

(check-sat)

