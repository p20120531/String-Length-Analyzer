(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_49910 () String)

(assert (= T_1 (= "-" var_0xINPUT_49910)))
(assert T_1)

(check-sat)

