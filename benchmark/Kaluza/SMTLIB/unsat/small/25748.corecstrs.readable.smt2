(set-logic QF_S)

(declare-fun T1_3 () String)
(declare-fun T2_3 () String)
(declare-fun T_1 () String)
(declare-fun var_0xINPUT_12712 () String)

(assert (= T_1 (str.++ T1_3 T2_3)))
(assert (= T2_3 "//3.ig.gmodules.com/gadgets/makeRequest?refresh=3600&url=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fhtml%2FSessionManager.action%3Fuser_id%3D%26type%3Dis_active_overwrite%26session_id%3D%26sid%3D0.8207011582573405&httpMethod=GET&headers=&postData=&authz=&st=&contentType=TEXT&numEntries=3&getSummaries=false&signOwner=true&signViewer=true&gadget=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fgadget%2FMyListy.xml&container=ig&bypassSpecCache=0"))
(assert (= T1_3 var_0xINPUT_12712))

(check-sat)

