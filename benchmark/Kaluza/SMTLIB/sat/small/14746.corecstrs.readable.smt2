(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_152672 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_152672))))
(assert T_1)

(check-sat)

