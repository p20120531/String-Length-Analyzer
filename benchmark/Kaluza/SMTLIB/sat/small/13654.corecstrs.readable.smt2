(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun var_0xINPUT_160703 () String)

(assert (= T_1 (not (= "cm6AfB9De3" var_0xINPUT_160703))))
(assert T_1)

(check-sat)

