(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_14858 () String)

(assert (= T_1 (= "-" var_0xINPUT_14858)))
(assert T_1)

(check-sat)

