(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_213171 () String)

(assert (= T_1 (not (= "" var_0xINPUT_213171))))
(assert T_1)
(assert (= T_2 (= var_0xINPUT_213171 "array")))
(assert T_2)

(check-sat)

