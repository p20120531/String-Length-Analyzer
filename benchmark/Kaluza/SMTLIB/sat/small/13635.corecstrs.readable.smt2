(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_134785 () String)

(assert (= T_1 (not (= "YY8CBEMy6Q" var_0xINPUT_134785))))
(assert T_1)

(check-sat)

