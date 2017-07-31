(set-logic QF_S)

(declare-fun PCTEMP_LHS_1 () String)
(declare-fun PCTEMP_LHS_2 () String)
(declare-fun PCTEMP_LHS_3 () String)
(declare-fun PCTEMP_LHS_4 () String)
(declare-fun PCTEMP_LHS_5 () String)
(declare-fun PCTEMP_LHS_6 () String)
(declare-fun PCTEMP_LHS_7 () String)
(declare-fun PCTEMP_LHS_7_idx_0 () String)
(declare-fun T1_33 () String)
(declare-fun T2_33 () String)
(declare-fun T_10 () Bool)
(declare-fun T_11 () Bool)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_14 () Bool)
(declare-fun T_15 () Bool)
(declare-fun T_16 () Bool)
(declare-fun T_17 () Bool)
(declare-fun T_18 () String)
(declare-fun T_1a () Int)
(declare-fun T_1b () Bool)
(declare-fun T_1c () String)
(declare-fun T_1d () String)
(declare-fun T_1e () Bool)
(declare-fun T_1f () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_b () Bool)
(declare-fun T_c () Bool)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)

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
(assert (= T_8 (= "" PCTEMP_LHS_7)))
(assert T_8)
(assert (= T_9 (not (= PCTEMP_LHS_7 "string"))))
(assert (= T_a (not T_9)))
(assert T_a)
(assert (= T_b (not (= PCTEMP_LHS_7 "string"))))
(assert (= T_c (not T_b)))
(assert T_c)
(assert (= T_d (not (= PCTEMP_LHS_7 "string"))))
(assert (= T_e (not T_d)))
(assert T_e)
(assert (= T_f (not (= PCTEMP_LHS_7 "string"))))
(assert (= T_10 (not T_f)))
(assert T_10)
(assert (= T_11 (not (= "" PCTEMP_LHS_7))))
(assert T_11)
(assert (= T_13 (not T_12)))
(assert T_13)
(assert (= T_15 (not T_14)))
(assert T_15)
(assert (= T_17 (not T_16)))
(assert T_17)
(assert (= T_18 (str.++ T1_33 T2_33)))
(assert (= T2_33 PCTEMP_LHS_7))
(assert (= T1_33 ""))
(assert (= T_1a (str.len PCTEMP_LHS_7)))
(assert (= T_1b (< 0 T_1a)))
(assert T_1b)
(assert (= T_1c PCTEMP_LHS_7_idx_0))
(assert (= T_1e (not (= "" T_1d))))
(assert (= T_1f (not T_1e)))
(assert T_1f)

(check-sat)

