(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_228121 () String)

(assert (= T_1 (not (= "" var_0xINPUT_228121))))
(assert T_1)
(assert (= T_2 (= var_0xINPUT_228121 "array")))
(assert T_2)

(check-sat)

