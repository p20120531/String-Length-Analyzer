(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_160798 () String)

(assert (= T_1 (not (= "EMy6QEb2YT" var_0xINPUT_160798))))
(assert T_1)

(check-sat)

