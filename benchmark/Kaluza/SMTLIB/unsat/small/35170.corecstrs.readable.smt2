(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_5002 () String)

(assert (= T_1 (not (= "" var_0xINPUT_5002))))
(assert T_1)

(check-sat)

