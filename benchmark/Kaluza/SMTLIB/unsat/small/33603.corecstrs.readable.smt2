(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_78597 () String)

(assert (= T_1 (not (= "" var_0xINPUT_78597))))
(assert T_1)

(check-sat)

