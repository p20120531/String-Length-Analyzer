(set-logic QF_S)

(declare-fun PCTEMP_LHS_1_idx_0 () String)
(declare-fun T0_2 () String)
(declare-fun var_0xINPUT_35882 () String)

(assert (= T0_2 PCTEMP_LHS_1_idx_0))
(assert (= T0_2 var_0xINPUT_35882))

(check-sat)

