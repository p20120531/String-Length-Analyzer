(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_2 () Bool)
(declare-fun var_0xINPUT_140054 () String)

(assert (= T_1 (not (= "2vbcJ9lN6J" var_0xINPUT_140054))))
(assert (= T_2 (not T_1)))
(assert T_2)

(check-sat)

