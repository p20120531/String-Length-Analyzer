(set-logic QF_S)
(declare-fun T_11 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_14 () Bool)
(declare-fun T_SELECT_4 () Bool)
(declare-fun T_SELECT_5 () Bool)
(declare-fun I0_23 () Int)
(declare-fun I0_29 () Int)
(declare-fun I1_29 () Int)
(declare-fun I2_29 () Int)
(declare-fun PCTEMP_LHS_4 () Int)
(declare-fun PCTEMP_LHS_5 () Int)
(declare-fun T_15 () Int)
(declare-fun T_16 () Int)
(declare-fun T0_23 () String)
(declare-fun T1_23 () String)
(declare-fun T2_23 () String)
(declare-fun T3_23 () String)
(declare-fun T4_23 () String)
(declare-fun T5_23 () String)
(declare-fun var_0xINPUT_131094 () String)
(assert (= var_0xINPUT_131094 (str.++ T0_23 T1_23)))
(assert (= T1_23 (str.++ T2_23 T3_23)))
(assert (= T2_23 (str.++ T4_23 T5_23)))
(assert (not (str.in.re T4_23 (str.to.re ";"))))
(assert (= T5_23 ";"))
(assert T_14)
(assert (= T_14 (not T_13)))
(assert (= T_13 (< PCTEMP_LHS_5 0)))
(assert (= T_SELECT_4 (not (= PCTEMP_LHS_4 (- 1)))))
(assert T_11)
(assert (= T_11 (< (- 1) PCTEMP_LHS_4)))
(assert (= PCTEMP_LHS_5 (+ I0_23 PCTEMP_LHS_4)))
(assert (= T_15 (+ PCTEMP_LHS_4 6)))
(assert (= T_16 (+ T_15 1)))
(assert (>= T_16 0))
(assert (>= PCTEMP_LHS_5 T_16))
(assert (= I0_29 (- PCTEMP_LHS_5 T_16)))
(assert (= I2_29 I1_29))
(assert (<= PCTEMP_LHS_5 I1_29))
(assert (= T_SELECT_5 (not (= PCTEMP_LHS_5 (- 1)))))
(assert (= PCTEMP_LHS_4 (str.len T0_23))) ; len 0
(assert (= I0_23 (str.len T4_23))) ; len 1
(assert T_SELECT_5)
(check-sat)