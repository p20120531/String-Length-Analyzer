(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_159968 () String)

(assert (= T_1 (= "https:" var_0xINPUT_159968)))
(assert T_1)

(check-sat)

