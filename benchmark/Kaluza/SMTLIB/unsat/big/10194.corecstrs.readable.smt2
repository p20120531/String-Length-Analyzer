(set-logic QF_S)

(declare-fun I0_13 () Int)
(declare-fun I0_20 () Int)
(declare-fun I0_27 () Int)
(declare-fun I0_34 () Int)
(declare-fun I0_41 () Int)
(declare-fun I0_6 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun PCTEMP_LHS_3 () Int)
(declare-fun PCTEMP_LHS_4 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun PCTEMP_LHS_6 () Int)
(declare-fun T0_13 () String)
(declare-fun T0_20 () String)
(declare-fun T0_27 () String)
(declare-fun T0_34 () String)
(declare-fun T0_41 () String)
(declare-fun T0_6 () String)
(declare-fun T1_13 () String)
(declare-fun T1_20 () String)
(declare-fun T1_27 () String)
(declare-fun T1_34 () String)
(declare-fun T1_41 () String)
(declare-fun T1_6 () String)
(declare-fun T2_13 () String)
(declare-fun T2_20 () String)
(declare-fun T2_27 () String)
(declare-fun T2_34 () String)
(declare-fun T2_41 () String)
(declare-fun T2_6 () String)
(declare-fun T3_13 () String)
(declare-fun T3_20 () String)
(declare-fun T3_27 () String)
(declare-fun T3_34 () String)
(declare-fun T3_41 () String)
(declare-fun T3_6 () String)
(declare-fun T4_13 () String)
(declare-fun T4_20 () String)
(declare-fun T4_27 () String)
(declare-fun T4_34 () String)
(declare-fun T4_41 () String)
(declare-fun T4_6 () String)
(declare-fun T5_13 () String)
(declare-fun T5_20 () String)
(declare-fun T5_27 () String)
(declare-fun T5_34 () String)
(declare-fun T5_41 () String)
(declare-fun T5_6 () String)
(declare-fun T_1 () Bool)
(declare-fun T_10 () Bool)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_14 () Bool)
(declare-fun T_15 () Bool)
(declare-fun T_16 () Bool)
(declare-fun T_18 () Bool)
(declare-fun T_19 () Bool)
(declare-fun T_1a () Bool)
(declare-fun T_1b () Bool)
(declare-fun T_1c () Bool)
(declare-fun T_1e () Bool)
(declare-fun T_1f () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_20 () Bool)
(declare-fun T_21 () Bool)
(declare-fun T_22 () Bool)
(declare-fun T_24 () Bool)
(declare-fun T_25 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_SELECT_3 () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun T_SELECT_6 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_c () Bool)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun var_0xINPUT_27554 () String)

(assert (= T_1 (not (= "" var_0xINPUT_27554))))
(assert T_1)
(assert (= T_2 (= "-" var_0xINPUT_27554)))
(assert (= T_3 (not T_2)))
(assert T_3)
(assert (= T_4 (= "" var_0xINPUT_27554)))
(assert T_4)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_6 0))(= var_0xINPUT_27554 (str.++ T0_6 T1_6))(= I0_6 (str.len T4_6))(= 0 (str.len T0_6))(= T1_6 (str.++ T2_6 T3_6))(= T2_6 (str.++ T4_6 T5_6))(= T5_6 "__utma=16886264.")(not (str.in.re T4_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_27554 (str.++ T0_6 T1_6))(= 0 (str.len T0_6))(not (str.in.re T1_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_7 (not T_6)))
(assert T_7)
(assert (= T_8 (= "-" var_0xINPUT_27554)))
(assert (= T_9 (not T_8)))
(assert T_9)
(assert (= T_a (= "" var_0xINPUT_27554)))
(assert T_a)
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_13 0))(= var_0xINPUT_27554 (str.++ T0_13 T1_13))(= I0_13 (str.len T4_13))(= 0 (str.len T0_13))(= T1_13 (str.++ T2_13 T3_13))(= T2_13 (str.++ T4_13 T5_13))(= T5_13 "__utmb=16886264")(not (str.in.re T4_13 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "b") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4"))))) 
	(and (= PCTEMP_LHS_2 (- 1))(= var_0xINPUT_27554 (str.++ T0_13 T1_13))(= 0 (str.len T0_13))(not (str.in.re T1_13 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "b") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4")))))))
(assert (= T_c (< (- 1) PCTEMP_LHS_2)))
(assert (= T_d (not T_c)))
(assert T_d)
(assert (= T_e (= "-" var_0xINPUT_27554)))
(assert (= T_f (not T_e)))
(assert T_f)
(assert (= T_10 (= "" var_0xINPUT_27554)))
(assert T_10)
(assert (= T_SELECT_3 (not (= PCTEMP_LHS_3 (- 1)))))
(assert (ite T_SELECT_3 
	(and (= PCTEMP_LHS_3 (+ I0_20 0))(= var_0xINPUT_27554 (str.++ T0_20 T1_20))(= I0_20 (str.len T4_20))(= 0 (str.len T0_20))(= T1_20 (str.++ T2_20 T3_20))(= T2_20 (str.++ T4_20 T5_20))(= T5_20 "__utmc=16886264")(not (str.in.re T4_20 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "c") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4"))))) 
	(and (= PCTEMP_LHS_3 (- 1))(= var_0xINPUT_27554 (str.++ T0_20 T1_20))(= 0 (str.len T0_20))(not (str.in.re T1_20 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "c") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4")))))))
(assert (= T_12 (< (- 1) PCTEMP_LHS_3)))
(assert (= T_13 (not T_12)))
(assert T_13)
(assert (= T_14 (= "-" var_0xINPUT_27554)))
(assert (= T_15 (not T_14)))
(assert T_15)
(assert (= T_16 (= "" var_0xINPUT_27554)))
(assert T_16)
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_4 (- 1)))))
(assert (ite T_SELECT_4 
	(and (= PCTEMP_LHS_4 (+ I0_27 0))(= var_0xINPUT_27554 (str.++ T0_27 T1_27))(= I0_27 (str.len T4_27))(= 0 (str.len T0_27))(= T1_27 (str.++ T2_27 T3_27))(= T2_27 (str.++ T4_27 T5_27))(= T5_27 "__utmx=16886264")(not (str.in.re T4_27 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "x") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4"))))) 
	(and (= PCTEMP_LHS_4 (- 1))(= var_0xINPUT_27554 (str.++ T0_27 T1_27))(= 0 (str.len T0_27))(not (str.in.re T1_27 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "x") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4")))))))
(assert (= T_18 (< (- 1) PCTEMP_LHS_4)))
(assert (= T_19 (not T_18)))
(assert T_19)
(assert (= T_1a (= "-" var_0xINPUT_27554)))
(assert (= T_1b (not T_1a)))
(assert T_1b)
(assert (= T_1c (= "" var_0xINPUT_27554)))
(assert T_1c)
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_5 (- 1)))))
(assert (ite T_SELECT_5 
	(and (= PCTEMP_LHS_5 (+ I0_34 0))(= var_0xINPUT_27554 (str.++ T0_34 T1_34))(= I0_34 (str.len T4_34))(= 0 (str.len T0_34))(= T1_34 (str.++ T2_34 T3_34))(= T2_34 (str.++ T4_34 T5_34))(= T5_34 "__utmz=16886264.")(not (str.in.re T4_34 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_5 (- 1))(= var_0xINPUT_27554 (str.++ T0_34 T1_34))(= 0 (str.len T0_34))(not (str.in.re T1_34 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_1e (< (- 1) PCTEMP_LHS_5)))
(assert (= T_1f (not T_1e)))
(assert T_1f)
(assert (= T_20 (= "-" var_0xINPUT_27554)))
(assert (= T_21 (not T_20)))
(assert T_21)
(assert (= T_22 (= "" var_0xINPUT_27554)))
(assert T_22)
(assert (= T_SELECT_6 (not (= PCTEMP_LHS_6 (- 1)))))
(assert (ite T_SELECT_6 
	(and (= PCTEMP_LHS_6 (+ I0_41 0))(= var_0xINPUT_27554 (str.++ T0_41 T1_41))(= I0_41 (str.len T4_41))(= 0 (str.len T0_41))(= T1_41 (str.++ T2_41 T3_41))(= T2_41 (str.++ T4_41 T5_41))(= T5_41 "__utmv=16886264.")(not (str.in.re T4_41 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "v") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_6 (- 1))(= var_0xINPUT_27554 (str.++ T0_41 T1_41))(= 0 (str.len T0_41))(not (str.in.re T1_41 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "v") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_24 (< (- 1) PCTEMP_LHS_6)))
(assert (= T_25 (not T_24)))
(assert T_25)

(check-sat)

