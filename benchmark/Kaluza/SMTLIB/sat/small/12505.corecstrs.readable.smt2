(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_202736 () String)

(assert (= T_1 (= var_0xINPUT_202736 "")))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

