(set-logic QF_S)
(declare-fun parameter () String)
(declare-fun a () String)
(declare-fun index () Int)
(declare-fun str1 () String)
(declare-fun index1 () Int)
(declare-fun sli () String)
(declare-fun index2 () Int)
;(assert (= parameter "eOfferCode") )
(assert (= index (str.indexof a "eOfferCode" 0) ) )
(assert (str.contains a "eOfferCode") ) ; for indexof
(assert (not (= index (- 0 1) ) ) )
(assert (= str1 (str.substr a index (- (str.len a) index) ) ) )
;(assert (= index1 (str.indexof str1 "&" 0) ) )
(assert (not (str.contains str1 "&") ) ) ; for indexof
(assert (= index1 (- 0 1) ) ) ; for indexof
(assert (= index1 (- 0 1) ) )
;(assert (= index2 (str.indexof str1 "=" 0) ) )
(assert (not (str.contains str1 "=") ) ) ; for indexof
(assert (= index2 (- 0 1) ) ) ; for indexof
(assert (= sli (str.substr str1 (+ index2 1) (- (str.len str1) (+ index2 1) ) ) ) )
(assert (not (= sli "") ) )
(check-sat)
