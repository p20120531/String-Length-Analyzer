(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_133205 () String)

(assert (= T_1 (not (= "ywUN4ZIDAC" var_0xINPUT_133205))))
(assert T_1)

(check-sat)
