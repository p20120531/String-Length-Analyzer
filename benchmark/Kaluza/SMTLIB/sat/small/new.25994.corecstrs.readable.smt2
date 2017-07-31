(set-logic QF_S)

(declare-fun I0_2 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_2 () String)
(declare-fun T1_2 () String)
(declare-fun T2_2 () String)
(declare-fun T3_2 () String)
(declare-fun T4_2 () String)
(declare-fun T5_2 () String)
(declare-fun T_2 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun var_0xINPUT_15413 () String)

(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_2 0))(= var_0xINPUT_15413 (str.++ T0_2 T1_2))(= I0_2 (str.len T4_2))(= 0 (str.len T0_2))(= T1_2 (str.++ T2_2 T3_2))(= T2_2 (str.++ T4_2 T5_2))(= T5_2 "GoogleAdServingTest=")(not (str.in.re T4_2 (re.++ (str.to.re "G") (str.to.re "o") (str.to.re "o") (str.to.re "g") (str.to.re "l") (str.to.re "e") (str.to.re "A") (str.to.re "d") (str.to.re "S") (str.to.re "e") (str.to.re "r") (str.to.re "v") (str.to.re "i") (str.to.re "n") (str.to.re "g") (str.to.re "T") (str.to.re "e") (str.to.re "s") (str.to.re "t") (str.to.re "="))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_15413 (str.++ T0_2 T1_2))(= 0 (str.len T0_2))(not (str.in.re T1_2 (re.++ (str.to.re "G") (str.to.re "o") (str.to.re "o") (str.to.re "g") (str.to.re "l") (str.to.re "e") (str.to.re "A") (str.to.re "d") (str.to.re "S") (str.to.re "e") (str.to.re "r") (str.to.re "v") (str.to.re "i") (str.to.re "n") (str.to.re "g") (str.to.re "T") (str.to.re "e") (str.to.re "s") (str.to.re "t") (str.to.re "=")))))))
(assert (= T_2 (not (= PCTEMP_LHS_1 (- 1)))))
(assert T_2)

(check-sat)

