(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_137662 () String)

(assert (= T_1 (= var_0xINPUT_137662 "xs3SCcPC7Y")))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

