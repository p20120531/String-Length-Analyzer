(set-logic QF_S)

(declare-fun T_1 () Int)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_254142 () String)

(assert (= T_1 (str.len var_0xINPUT_254142)))
(assert (= T_2 (= T_1 0)))
(assert T_2)

(check-sat)

