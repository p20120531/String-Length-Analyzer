(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)
(declare-fun var_0xINPUT_150022 () String)

(assert (= PCTEMP_LHS_1 var_0xINPUT_150022))
(assert (not (str.in.re PCTEMP_LHS_1 (str.to.re "%"))))

(check-sat)

