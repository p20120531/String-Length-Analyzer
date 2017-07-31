(set-logic QF_S)

(declare-fun I0_2 () Int)
(declare-fun I0_4 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_2 () String)
(declare-fun T0_4 () String)
(declare-fun T1_2 () String)
(declare-fun T1_4 () String)
(declare-fun T2_2 () String)
(declare-fun T2_4 () String)
(declare-fun T3_2 () String)
(declare-fun T3_4 () String)
(declare-fun T4_2 () String)
(declare-fun T4_4 () String)
(declare-fun T5_2 () String)
(declare-fun T5_4 () String)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun var_0xINPUT_54305 () String)

(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_2 0))(= var_0xINPUT_54305 (str.++ T0_2 T1_2))(= I0_2 (str.len T4_2))(= 0 (str.len T0_2))(= T1_2 (str.++ T2_2 T3_2))(= T2_2 (str.++ T4_2 T5_2))(= T5_2 "?")(not (str.in.re T4_2 (str.to.re "?")))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_54305 (str.++ T0_2 T1_2))(= 0 (str.len T0_2))(not (str.in.re T1_2 (str.to.re "?"))))))
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_4 0))(= var_0xINPUT_54305 (str.++ T0_4 T1_4))(= I0_4 (str.len T4_4))(= 0 (str.len T0_4))(= T1_4 (str.++ T2_4 T3_4))(= T2_4 (str.++ T4_4 T5_4))(= T5_4 "#")(not (str.in.re T4_4 (str.to.re "#")))) 
	(and (= PCTEMP_LHS_2 (- 1))(= var_0xINPUT_54305 (str.++ T0_4 T1_4))(= 0 (str.len T0_4))(not (str.in.re T1_4 (str.to.re "#"))))))
(assert (= T_3 (= PCTEMP_LHS_2 (- 1))))
(assert (= T_4 (not T_3)))
(assert T_4)

(check-sat)

