(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_133442 () String)

(assert (= T_1 (= "BRBa46q0I8" var_0xINPUT_133442)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

