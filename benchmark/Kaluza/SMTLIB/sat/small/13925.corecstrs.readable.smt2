(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_125339 () String)

(assert (= T_1 (= "https:" var_0xINPUT_125339)))
(assert T_1)

(check-sat)

