(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_190942 () String)

(assert (= T_1 (= var_0xINPUT_190942 "Search")))
(assert T_1)

(check-sat)

