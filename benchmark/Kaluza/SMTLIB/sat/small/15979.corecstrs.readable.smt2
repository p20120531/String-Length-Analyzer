(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_206325 () String)

(assert (= T_1 (= var_0xINPUT_206325 "Type")))
(assert T_1)

(check-sat)

