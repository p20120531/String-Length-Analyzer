(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () Bool)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun PCTEMP_LHS_4_idx_0 () String)
(declare-fun PCTEMP_LHS_5 () String)
(declare-fun T0_24 () String)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_14 () Bool)
(declare-fun T_15 () Bool)
(declare-fun T_16 () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Int)
(declare-fun T_4 () Int)
(declare-fun T_6 () Int)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Int)
(declare-fun T_a () Bool)
(declare-fun T_b () Bool)
(declare-fun T_c () Int)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun var_0xINPUT_250044 () String)

(assert (= T_2 (not PCTEMP_LHS_1)))
(assert T_2)
(assert (= T_3 (str.len var_0xINPUT_250044)))
(assert (= T_4 (div T_3 10)))
(assert (= T_6 (str.len var_0xINPUT_250044)))
(assert (= T_7 (< 70 T_6)))
(assert (= T_8 (not T_7)))
(assert T_8)
(assert (= T_9 (str.len var_0xINPUT_250044)))
(assert (= T_a (< 70 T_9)))
(assert (= T_b (not T_a)))
(assert T_b)
(assert (= T_c (str.len var_0xINPUT_250044)))
(assert (= T_d (< 70 T_c)))
(assert (= T_e (not T_d)))
(assert T_e)
(assert (= PCTEMP_LHS_3 var_0xINPUT_250044))
(assert (= T0_24 PCTEMP_LHS_4_idx_0))
(assert (= T0_24 PCTEMP_LHS_3))
(assert (= T_12 (= PCTEMP_LHS_5 "[object")))
(assert (= T_13 (not T_12)))
(assert T_13)
(assert (= T_15 (not T_14)))
(assert T_15)
(assert T_16)

(check-sat)

