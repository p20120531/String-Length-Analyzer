(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_159446 () String)

(assert (= T_1 (not (= "AUEAICDdfN" var_0xINPUT_159446))))
(assert T_1)

(check-sat)

