(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_274689 () String)

(assert (= T_1 (not (= "" var_0xINPUT_274689))))
(assert T_1)

(check-sat)

