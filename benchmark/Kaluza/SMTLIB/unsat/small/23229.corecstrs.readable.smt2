(set-logic QF_S)

(declare-fun I0_5 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_5 () String)
(declare-fun T1_5 () String)
(declare-fun T2_5 () String)
(declare-fun T3_5 () String)
(declare-fun T4_5 () String)
(declare-fun T5_5 () String)
(declare-fun T_1 () Bool)
(declare-fun T_10 () Bool)
(declare-fun T_11 () String)
(declare-fun T_12 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_7 () String)
(declare-fun T_8 () Bool)
(declare-fun T_9 () String)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_b () String)
(declare-fun T_c () Bool)
(declare-fun T_d () String)
(declare-fun T_e () Bool)
(declare-fun T_f () String)
(declare-fun var_0xINPUT_96464 () String)

(assert (= T_1 (= "-" var_0xINPUT_96464)))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (= "" var_0xINPUT_96464)))
(assert T_3)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_5 0))(= var_0xINPUT_96464 (str.++ T0_5 T1_5))(= I0_5 (str.len T4_5))(= 0 (str.len T0_5))(= T1_5 (str.++ T2_5 T3_5))(= T2_5 (str.++ T4_5 T5_5))(= T5_5 "__utmz=16886264.")(not (str.in.re T4_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_96464 (str.++ T0_5 T1_5))(= 0 (str.len T0_5))(not (str.in.re T1_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_5 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_6 (not T_5)))
(assert T_6)
(assert (= T_8 (= "-" T_7)))
(assert T_8)
(assert (= T_a (= "-" T_9)))
(assert T_a)
(assert (= T_c (= "-" T_b)))
(assert T_c)
(assert (= T_e (= "-" T_d)))
(assert T_e)
(assert (= T_10 (= "-" T_f)))
(assert T_10)
(assert (= T_12 (= "-" T_11)))
(assert T_12)

(check-sat)

