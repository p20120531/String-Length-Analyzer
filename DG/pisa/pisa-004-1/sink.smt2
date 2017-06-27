(set-logic QF_S)
(set-option :produce-models true)


(declare-fun s  () String)
(declare-fun filename_0  () String)
(declare-fun filename_1  () String)
(declare-fun filename_2  () String)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun tmpStr0 () String)
(declare-fun tmpStr1 () String)



(assert (= filename_0 s) )

(assert (str.contains filename_0 "/") )
(assert (= filename_0 (str.++ tmpStr0 (str.++ "/" tmpStr1) ) ) )
(assert (not (str.contains tmpStr1 "/") ) )
(assert (= i1 (str.len tmpStr0) ) )

(assert (= i2 (- (str.len filename_0) i1) ) )
(assert (= filename_1 (str.substr filename_0 i1 i2) ) )

(assert (= i3 (str.indexof filename_1 "." 0) ) )

(assert (not (= i3 (- 0 1) ) ) )
(assert (= filename_2 (str.substr filename_1 0 i3) ) )

(assert (str.contains filename_2 "../") )


(check-sat)
(get-model)
