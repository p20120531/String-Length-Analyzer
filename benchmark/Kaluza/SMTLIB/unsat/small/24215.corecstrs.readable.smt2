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
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun var_0xINPUT_47771 () String)

(assert (= T_1 (= "-" var_0xINPUT_47771)))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (= "" var_0xINPUT_47771)))
(assert T_3)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_5 0))(= var_0xINPUT_47771 (str.++ T0_5 T1_5))(= I0_5 (str.len T4_5))(= 0 (str.len T0_5))(= T1_5 (str.++ T2_5 T3_5))(= T2_5 (str.++ T4_5 T5_5))(= T5_5 "__utmz=218069774.")(not (str.in.re T4_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "2") (str.to.re "1") (str.to.re "8") (str.to.re "0") (str.to.re "6") (str.to.re "9") (str.to.re "7") (str.to.re "7") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_47771 (str.++ T0_5 T1_5))(= 0 (str.len T0_5))(not (str.in.re T1_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "z") (str.to.re "=") (str.to.re "2") (str.to.re "1") (str.to.re "8") (str.to.re "0") (str.to.re "6") (str.to.re "9") (str.to.re "7") (str.to.re "7") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_5 (< (- 1) PCTEMP_LHS_1)))
(assert (= T_6 (not T_5)))
(assert T_6)

(check-sat)

