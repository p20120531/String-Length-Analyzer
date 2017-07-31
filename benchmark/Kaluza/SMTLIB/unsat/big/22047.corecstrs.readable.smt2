(set-logic QF_S)

(declare-fun I0_6 () Int)
(declare-fun PCTEMP_LHS_1 () String)
(declare-fun PCTEMP_LHS_2 () Int)
(declare-fun T0_6 () String)
(declare-fun T1_5 () String)
(declare-fun T1_6 () String)
(declare-fun T2_5 () String)
(declare-fun T2_6 () String)
(declare-fun T3_6 () String)
(declare-fun T4_6 () String)
(declare-fun T5_6 () String)
(declare-fun T_2 () String)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_SELECT_1 () Bool)

(assert (not (str.in.re PCTEMP_LHS_1 (re.++ (str.to.re "%") (str.to.re "h") (str.to.re "o") (str.to.re "s") (str.to.re "t") (str.to.re "%")))))
(assert (= PCTEMP_LHS_1 "//%host%/gadgets/makeRequest"))
(assert (= T_2 (str.++ T1_5 T2_5)))
(assert (= T2_5 "?refresh=3600&url=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fhtml%2FSessionManager.action%3Fuser_id%3D%26type%3Dis_active_overwrite%26session_id%3D%26sid%3D0.0965873145941234&httpMethod=GET&headers=&postData=&authz=&st=&contentType=TEXT&numEntries=3&getSummaries=false&signOwner=true&signViewer=true&gadget=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fgadget%2FMyListy.xml&container=ig&bypassSpecCache=0"))
(assert (= T1_5 PCTEMP_LHS_1))
(assert (= T_SELECT_1 (not (= PCTEMP_LHS_2 (- 1)))))
(assert (ite T_SELECT_1 
	(and (= PCTEMP_LHS_2 (+ I0_6 0))(= T_2 (str.++ T0_6 T1_6))(= I0_6 (str.len T4_6))(= 0 (str.len T0_6))(= T1_6 (str.++ T2_6 T3_6))(= T2_6 (str.++ T4_6 T5_6))(= T5_6 "//")(not (str.in.re T4_6 (re.++ (str.to.re "/") (str.to.re "/"))))) 
	(and (= PCTEMP_LHS_2 (- 1))(= T_2 (str.++ T0_6 T1_6))(= 0 (str.len T0_6))(not (str.in.re T1_6 (re.++ (str.to.re "/") (str.to.re "/")))))))
(assert (= T_4 (= PCTEMP_LHS_2 0)))
(assert (= T_5 (not T_4)))
(assert T_5)

(check-sat)

