(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_110525 () String)

(assert (= T_1 (= var_0xINPUT_110525 "Type")))
(assert T_1)

(check-sat)

