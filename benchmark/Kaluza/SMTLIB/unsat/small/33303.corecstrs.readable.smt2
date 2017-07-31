(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)

(assert (not (str.in.re PCTEMP_LHS_1 (re.++ (str.to.re "%") (str.to.re "h") (str.to.re "o") (str.to.re "s") (str.to.re "t") (str.to.re "%")))))

(check-sat)

