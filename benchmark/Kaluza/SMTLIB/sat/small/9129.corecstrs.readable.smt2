(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_80591 () String)

(assert (= T_1 (not (= "6JX7G3VKFq" var_0xINPUT_80591))))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

