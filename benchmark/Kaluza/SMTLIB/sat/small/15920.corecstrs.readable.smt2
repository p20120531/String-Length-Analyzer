(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_40077 () String)

(assert (= T_1 (not (= "file:" var_0xINPUT_40077))))
(assert T_1)

(check-sat)

