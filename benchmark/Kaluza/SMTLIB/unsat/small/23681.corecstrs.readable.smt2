(set-logic QF_S)

(declare-fun T1_3 () String)
(declare-fun T2_3 () String)
(declare-fun T_1 () String)
(declare-fun var_0xINPUT_94747 () String)

(assert (= T_1 (str.++ T1_3 T2_3)))
(assert (= T2_3 "//3.ig.gmodules.com/gadgets/makeRequest?refresh=3600&url=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fhtml%2FDataManager.action%3Bjsessionid%3DEAA37477B6FD2AB38407DA5B413CA431%3Fuser_id%3D20100221063724212983%26type%3Dadd_row%26tab_index%3D0%26tab_name%3DListy%26data%3D81FUqFcPOs%26priority%3D3%26row_number%3D0%26sid%3D0.6872598513147243&httpMethod=GET&headers=&postData=&authz=&st=&contentType=DOM&numEntries=3&getSummaries=false&signOwner=true&signViewer=true&gadget=http%3A%2F%2Fwww.mylisty.net%2FListy%2Fgadget%2FMyListy.xml&container=ig&bypassSpecCache=0"))
(assert (= T1_3 var_0xINPUT_94747))

(check-sat)
