(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_108602 () String)

(assert (= T_1 (= "3u9pORxpWA" var_0xINPUT_108602)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)
