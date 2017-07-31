(set-logic QF_S)

(declare-fun I0_14 () Int)
(declare-fun I0_5 () Int)
(declare-fun I0_8 () Int)
(declare-fun I1_14 () Int)
(declare-fun I2_14 () Int)
(declare-fun M0_16 () String)
(declare-fun M1_16 () String)
(declare-fun M2_16 () String)
(declare-fun M3_16 () String)
(declare-fun M4_16 () String)
(declare-fun P0_16 () String)
(declare-fun P1_16 () String)
(declare-fun P2_16 () String)
(declare-fun P3_16 () String)
(declare-fun P4_16 () String)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun PCTEMP_LHS_4 () String)
(declare-fun PCTEMP_LHS_4_idx_0 () String)
(declare-fun PCTEMP_LHS_4_idx_1 () String)
(declare-fun PCTEMP_LHS_4_idx_2 () String)
(declare-fun PCTEMP_LHS_4_idx_3 () String)
(declare-fun PCTEMP_LHS_4_idx_4 () String)
(declare-fun PCTEMP_LHS_4_idx_5 () String)
(declare-fun T0_16 () String)
(declare-fun T0_5 () String)
(declare-fun T0_8 () String)
(declare-fun T1_14 () String)
(declare-fun T1_16 () String)
(declare-fun T1_5 () String)
(declare-fun T1_8 () String)
(declare-fun T2_14 () String)
(declare-fun T2_16 () String)
(declare-fun T2_5 () String)
(declare-fun T2_8 () String)
(declare-fun T3_14 () String)
(declare-fun T3_16 () String)
(declare-fun T3_5 () String)
(declare-fun T3_8 () String)
(declare-fun T4_16 () String)
(declare-fun T4_5 () String)
(declare-fun T4_8 () String)
(declare-fun T5_16 () String)
(declare-fun T5_5 () String)
(declare-fun T5_8 () String)
(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Int)
(declare-fun T_SELECT_1 () Bool)
(declare-fun T_SELECT_2 () Bool)
(declare-fun T_a () Int)
(declare-fun T_d () Int)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun var_0xINPUT_23669 () String)

(assert (= T_1 (= "-" var_0xINPUT_23669)))
(assert (= T_2 (not T_1)))
(assert T_2)
(assert (= T_3 (= "" var_0xINPUT_23669)))
(assert T_3)
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_1 (+ I0_5 0))(= var_0xINPUT_23669 (str.++ T0_5 T1_5))(= I0_5 (str.len T4_5))(= 0 (str.len T0_5))(= T1_5 (str.++ T2_5 T3_5))(= T2_5 (str.++ T4_5 T5_5))(= T5_5 "__utma=16886264.")(not (str.in.re T4_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re "."))))) 
	(and (= PCTEMP_LHS_1 (- 1))(= var_0xINPUT_23669 (str.++ T0_5 T1_5))(= 0 (str.len T0_5))(not (str.in.re T1_5 (re.++ (str.to.re "_") (str.to.re "_") (str.to.re "u") (str.to.re "t") (str.to.re "m") (str.to.re "a") (str.to.re "=") (str.to.re "1") (str.to.re "6") (str.to.re "8") (str.to.re "8") (str.to.re "6") (str.to.re "2") (str.to.re "6") (str.to.re "4") (str.to.re ".")))))))
(assert (= T_5 (< (- 1) PCTEMP_LHS_1)))
(assert T_5)
(assert (= T_SELECT_2 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_2 
	(and (= PCTEMP_LHS_2 (+ I0_8 PCTEMP_LHS_1))(= var_0xINPUT_23669 (str.++ T0_8 T1_8))(= I0_8 (str.len T4_8))(= PCTEMP_LHS_1 (str.len T0_8))(= T1_8 (str.++ T2_8 T3_8))(= T2_8 (str.++ T4_8 T5_8))(= T5_8 ";")(not (str.in.re T4_8 (str.to.re ";")))) 
	(and (= PCTEMP_LHS_2 (- 1))(= var_0xINPUT_23669 (str.++ T0_8 T1_8))(= PCTEMP_LHS_1 (str.len T0_8))(not (str.in.re T1_8 (str.to.re ";"))))))
(assert (= T_7 (< PCTEMP_LHS_2 0)))
(assert (= T_8 (not T_7)))
(assert T_8)
(assert (= T_9 (+ PCTEMP_LHS_1 6)))
(assert (= T_a (+ T_9 1)))
(assert (= I0_14 (- PCTEMP_LHS_2 T_a)))
(assert (>= T_a 0))
(assert (>= PCTEMP_LHS_2 T_a))
(assert (<= PCTEMP_LHS_2 I1_14))
(assert (= I2_14 I1_14))
(assert (= I0_14 (str.len PCTEMP_LHS_3)))
(assert (= var_0xINPUT_23669 (str.++ T1_14 T2_14)))
(assert (= T2_14 (str.++ PCTEMP_LHS_3 T3_14)))
(assert (= T_a (str.len T1_14)))
(assert (= I1_14 (str.len var_0xINPUT_23669)))
(assert (= T5_16 PCTEMP_LHS_4_idx_5))
(assert (= T0_16 PCTEMP_LHS_3))
(assert (= M4_16 "\\."))
(assert (not (str.in.re PCTEMP_LHS_4_idx_4 (str.to.re "."))))
(assert (= P4_16 (str.++ PCTEMP_LHS_4_idx_4 M4_16)))
(assert (= T4_16 (str.++ P4_16 T5_16)))
(assert (= M3_16 "\\."))
(assert (not (str.in.re PCTEMP_LHS_4_idx_3 (str.to.re "."))))
(assert (= P3_16 (str.++ PCTEMP_LHS_4_idx_3 M3_16)))
(assert (= T3_16 (str.++ P3_16 T4_16)))
(assert (= M2_16 "\\."))
(assert (not (str.in.re PCTEMP_LHS_4_idx_2 (str.to.re "."))))
(assert (= P2_16 (str.++ PCTEMP_LHS_4_idx_2 M2_16)))
(assert (= T2_16 (str.++ P2_16 T3_16)))
(assert (= M1_16 "\\."))
(assert (not (str.in.re PCTEMP_LHS_4_idx_1 (str.to.re "."))))
(assert (= P1_16 (str.++ PCTEMP_LHS_4_idx_1 M1_16)))
(assert (= T1_16 (str.++ P1_16 T2_16)))
(assert (= M0_16 "\\."))
(assert (not (str.in.re PCTEMP_LHS_4_idx_0 (str.to.re "."))))
(assert (= P0_16 (str.++ PCTEMP_LHS_4_idx_0 M0_16)))
(assert (= T0_16 (str.++ P0_16 T1_16)))
(assert (= T_d (str.len PCTEMP_LHS_4)))
(assert (= T_e (< T_d 4)))
(assert (= T_f (not T_e)))
(assert T_f)

(check-sat)

