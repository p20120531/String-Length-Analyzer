(set-logic QF_S)

(declare-fun I0_3 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_3 () String)
(declare-fun T1_3 () String)
(declare-fun T2_3 () String)
(declare-fun T3_3 () String)
(declare-fun T4_3 () String)
(declare-fun T5_3 () String)
(declare-fun T_1 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun var_0xINPUT_5121 () String)

(assert (= T_1 (not (= "" var_0xINPUT_5121))))
(assert T_1)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_3 0))(= var_0xINPUT_5121 (str.++ T0_3 T1_3))(= I0_3 (str.len T4_3))(= 0 (str.len T0_3))(= T1_3 (str.++ T2_3 T3_3))(= T2_3 (str.++ T4_3 T5_3))(= T5_3 "?")(not (str.in.re T4_3 (str.to.re "?")))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_5121 (str.++ T0_3 T1_3))(= 0 (str.len T0_3))(not (str.in.re T1_3 (str.to.re "?"))))))
(assert (= T_3 (= PCTEMP_LHS_1 (- 1))))
(assert T_3)

(check-sat)

