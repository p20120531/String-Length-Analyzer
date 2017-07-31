(set-logic QF_S)

(declare-fun I0_14 () Int)
(declare-fun I0_6 () Int)
(declare-fun I0_9 () Int)
(declare-fun I1_14 () Int)
(declare-fun I2_14 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun T0_6 () String)
(declare-fun T0_9 () String)
(declare-fun T1_14 () String)
(declare-fun T1_6 () String)
(declare-fun T1_9 () String)
(declare-fun T2_14 () String)
(declare-fun T2_6 () String)
(declare-fun T2_9 () String)
(declare-fun T3_14 () String)
(declare-fun T3_6 () String)
(declare-fun T3_9 () String)
(declare-fun T4_6 () String)
(declare-fun T4_9 () String)
(declare-fun T5_6 () String)
(declare-fun T5_9 () String)
(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_a () Int)
(declare-fun T_c () Bool)
(declare-fun T_d () Bool)
(declare-fun var_0xINPUT_37424 () String)

(assert (= T_1 (= "" var_0xINPUT_37424)))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (= var_0xINPUT_37424 "")))
(assert (= T_4 (not T_3)))
(assert T_4)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_6 0))(= var_0xINPUT_37424 (str.++ T0_6 T1_6))(= I0_6 (str.len T4_6))(= 0 (str.len T0_6))(= T1_6 (str.++ T2_6 T3_6))(= T2_6 (str.++ T4_6 T5_6))(= T5_6 "__utma=218069774.")(not (str.in.re T4_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "2") (str.to.re "1") (str.to.re "8") (str.to.re "0") (str.to.re "6") (str.to.re "9") (str.to.re "7") (str.to.re "7") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_37424 (str.++ T0_6 T1_6))(= 0 (str.len T0_6))(not (str.in.re T1_6 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "2") (str.to.re "1") (str.to.re "8") (str.to.re "0") (str.to.re "6") (str.to.re "9") (str.to.re "7") (str.to.re "7") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert T_6)
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_9 PCTEMP_LHS_1))(= var_0xINPUT_37424 (str.++ T0_9 T1_9))(= I0_9 (str.len T4_9))(= PCTEMP_LHS_1 (str.len T0_9))(= T1_9 (str.++ T2_9 T3_9))(= T2_9 (str.++ T4_9 T5_9))(= T5_9 ";")(not (str.in.re T4_9 (str.to.re ";")))) 
	(and (= PCTEMP_LHS_2 (- 1))(= var_0xINPUT_37424 (str.++ T0_9 T1_9))(= PCTEMP_LHS_1 (str.len T0_9))(not (str.in.re T1_9 (str.to.re ";"))))))
(assert (= T_8 (< PCTEMP_LHS_2 0)))
(assert (= T_9 (not T_8)))
(assert T_9)
(assert (= T_a (+ PCTEMP_LHS_1 7)))
(assert (= I0_14 (- PCTEMP_LHS_2 T_a)))
(assert (>= T_a 0))
(assert (>= PCTEMP_LHS_2 T_a))
(assert (<= PCTEMP_LHS_2 I1_14))
(assert (= I2_14 I1_14))
(assert (= I0_14 (str.len PCTEMP_LHS_3)))
(assert (= var_0xINPUT_37424 (str.++ T1_14 T2_14)))
(assert (= T2_14 (str.++ PCTEMP_LHS_3 T3_14)))
(assert (= T_a (str.len T1_14)))
(assert (= I1_14 (str.len var_0xINPUT_37424)))
(assert (= T_c (not (= PCTEMP_LHS_3 "-"))))
(assert (= T_d (not T_c)))
(assert T_d)

(check-sat)

