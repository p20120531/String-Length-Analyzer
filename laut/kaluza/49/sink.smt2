(set-logic QF_S)
(declare-fun T_6 () Bool)
(declare-fun T_SELECT_1 () Bool)
(declare-fun I0_6 () Int)
(declare-fun PCTEMP_LHS_1 () Int)
(declare-fun T0_6 () String)
(declare-fun T1_6 () String)
(declare-fun T2_6 () String)
(declare-fun T3_6 () String)
(declare-fun T4_6 () String)
(declare-fun T5_6 () String)
(declare-fun var_0xINPUT_8283 () String)
(assert (= var_0xINPUT_8283 (str.++ T0_6 T1_6)))
(assert (= T1_6 (str.++ T2_6 T3_6)))
(assert (= T2_6 (str.++ T4_6 T5_6)))
(assert (not (str.in.re T4_6 (str.to.re "GASO="))))
(assert (= T5_6 "GASO="))
(assert T_6)
(assert (= T_6 (< (- 1) PCTEMP_LHS_1)))
(assert (= PCTEMP_LHS_1 (+ I0_6 0)))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_1 (- 1)))))
(assert (= 0 (str.len T0_6))) ; len 0
(assert (= I0_6 (str.len T4_6))) ; len 1
(assert T_SELECT_1)
(check-sat)