(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_223294 () String)

(assert (= T_1 (not (= "YY8CBEMy6Q" var_0xINPUT_223294))))
(assert T_1)
(assert (= T_2 (not (= var_0xINPUT_223294 ""))))
(assert T_2)

(check-sat)

