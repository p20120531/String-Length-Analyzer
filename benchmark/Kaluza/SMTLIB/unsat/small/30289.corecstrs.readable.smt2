(set-logic QF_S)

(declare-fun T_1 () Int)
(declare-fun T_2 () Bool)
(declare-fun T_3 () Bool)
(declare-fun var_0xINPUT_178851 () String)

(assert (= T_1 (str.len var_0xINPUT_178851)))
(assert (= T_2 (< 0 T_1)))
(assert (= T_3 (not T_2)))
(assert T_3)

(check-sat)

