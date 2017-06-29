(set-logic QF_S)
(set-option :produce-models true)


(declare-fun s  () String)
(declare-fun filename_0  () String)
(declare-fun filename_1  () String)
(declare-fun filename_2  () String)
(declare-fun i0 () Int)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun tmpStr0 () String)
(declare-fun tmpStr1 () String)
(declare-fun tmpStr2 () String)


(assert (= filename_0 s) )

;(assert (= i0 (str.indexof filename_0 "/" 0) ) )
(assert (not (str.contains filename_0 "/") ) ) ; for indexof
(assert (= i0 (- 0 1) ) ) ; for indexof

(assert (= i0 (- 0 1) ) )
(assert (= filename_1 filename_0) )



;(assert (= i3 (str.indexof filename_1 "." 0) ) )
(assert (not (str.contains filename_1 ".") ) ) ; for indexof
(assert (= i3 (- 0 1) ) ) ; for indexof


(assert (= i3 (- 0 1) ) )
(assert (= filename_2 filename_1) )


(assert (str.contains filename_2 "../") )


(check-sat)
(get-model)
