(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_35349 () String)

(assert (= T_1 (= "https:" var_0xINPUT_35349)))
(assert T_1)

(check-sat)

