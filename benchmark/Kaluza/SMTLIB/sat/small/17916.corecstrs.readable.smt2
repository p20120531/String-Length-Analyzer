(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_157835 () String)

(assert (= T_1 (= "-" var_0xINPUT_157835)))
(assert T_1)

(check-sat)

