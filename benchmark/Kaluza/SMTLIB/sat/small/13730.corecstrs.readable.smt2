(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_42112 () String)

(assert (= T_1 (not (= "6JX7G3VKFq" var_0xINPUT_42112))))
(assert T_1)

(check-sat)

