(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_49115 () String)

(assert (= T_1 (not (= "" var_0xINPUT_49115))))
(assert T_1)

(check-sat)

