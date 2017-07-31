(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)
(declare-fun PCTEMP_LHS_2 () String)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun PCTEMP_LHS_4 () String)
(declare-fun PCTEMP_LHS_5 () String)
(declare-fun PCTEMP_LHS_6 () String)
(declare-fun PCTEMP_LHS_7 () String)
(declare-fun T_10 () String)
(declare-fun T_11 () Bool)
(declare-fun T_12 () Bool)
(declare-fun T_13 () String)
(declare-fun T_14 () String)
(declare-fun T_15 () Bool)
(declare-fun T_16 () Bool)
(declare-fun T_17 () String)
(declare-fun T_18 () String)
(declare-fun T_19 () Bool)
(declare-fun T_1a () Bool)
(declare-fun T_1b () String)
(declare-fun T_1c () String)
(declare-fun T_1d () Bool)
(declare-fun T_1e () Bool)
(declare-fun T_8 () String)
(declare-fun T_9 () String)
(declare-fun T_a () Bool)
(declare-fun T_b () String)
(declare-fun T_c () String)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () String)

(assert (not (str.in.re PCTEMP_LHS_1 (re.++ (str.to.re "%") (str.to.re "h") (str.to.re "o") (str.to.re "s") (str.to.re "t") (str.to.re "%")))))
(assert (= PCTEMP_LHS_1 "http://www.ig.gmodules.com/gadgets/proxy/refresh=%refresh%&container=%container%%rewriteMime%&gadget=%gadget%/%rawurl%"))
(assert (not (str.in.re PCTEMP_LHS_2 (re.++ (str.to.re "%") (str.to.re "r") (str.to.re "a") (str.to.re "w") (str.to.re "u") (str.to.re "r") (str.to.re "l") (str.to.re "%")))))
(assert (= PCTEMP_LHS_2 PCTEMP_LHS_1))
(assert (not (str.in.re PCTEMP_LHS_3 (re.++ (str.to.re "%") (str.to.re "r") (str.to.re "e") (str.to.re "f") (str.to.re "r") (str.to.re "e") (str.to.re "s") (str.to.re "h") (str.to.re "%")))))
(assert (= PCTEMP_LHS_3 PCTEMP_LHS_2))
(assert (not (str.in.re PCTEMP_LHS_4 (re.++ (str.to.re "%") (str.to.re "g") (str.to.re "a") (str.to.re "d") (str.to.re "g") (str.to.re "e") (str.to.re "t") (str.to.re "%")))))
(assert (= PCTEMP_LHS_4 PCTEMP_LHS_3))
(assert (not (str.in.re PCTEMP_LHS_5 (re.++ (str.to.re "%") (str.to.re "c") (str.to.re "o") (str.to.re "n") (str.to.re "t") (str.to.re "a") (str.to.re "i") (str.to.re "n") (str.to.re "e") (str.to.re "r") (str.to.re "%")))))
(assert (= PCTEMP_LHS_5 PCTEMP_LHS_4))
(assert (not (str.in.re PCTEMP_LHS_6 (re.++ (str.to.re "%") (str.to.re "r") (str.to.re "e") (str.to.re "w") (str.to.re "r") (str.to.re "i") (str.to.re "t") (str.to.re "e") (str.to.re "M") (str.to.re "i") (str.to.re "m") (str.to.re "e") (str.to.re "%")))))
(assert (= PCTEMP_LHS_6 PCTEMP_LHS_5))
(assert (not (str.in.re PCTEMP_LHS_7 (re.++ (str.to.re "%") (str.to.re "r") (str.to.re "e") (str.to.re "w") (str.to.re "r") (str.to.re "i") (str.to.re "t") (str.to.re "e") (str.to.re "M") (str.to.re "i") (str.to.re "m") (str.to.re "e") (str.to.re "%")))))
(assert (= PCTEMP_LHS_7 PCTEMP_LHS_6))
(assert (= T_8 (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_9 (str.++ T_8 ")")))
(assert (= T_a (= "" T_9)))
(assert T_a)
(assert (= T_b (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_c (str.++ T_b ")")))
(assert (= T_d (not (= T_c "string"))))
(assert (= T_e (not T_d)))
(assert T_e)
(assert (= T_f (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_10 (str.++ T_f ")")))
(assert (= T_11 (not (= T_10 "string"))))
(assert (= T_12 (not T_11)))
(assert T_12)
(assert (= T_13 (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_14 (str.++ T_13 ")")))
(assert (= T_15 (not (= T_14 "string"))))
(assert (= T_16 (not T_15)))
(assert T_16)
(assert (= T_17 (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_18 (str.++ T_17 ")")))
(assert (= T_19 (not (= T_18 "string"))))
(assert (= T_1a (not T_19)))
(assert T_1a)
(assert (= T_1b (str.++ "transparent" PCTEMP_LHS_7)))
(assert (= T_1c (str.++ T_1b ")")))
(assert (= T_1d (not (= "" T_1c))))
(assert (= T_1e (not T_1d)))
(assert T_1e)

(check-sat)

