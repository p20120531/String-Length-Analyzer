(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_125250 () String)

(assert (= T_1 (= "OvAGN9wPDO" var_0xINPUT_125250)))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)
