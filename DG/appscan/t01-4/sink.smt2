(set-logic QF_S)
(set-option :produce-models true)

;(declare-fun parameter () String)
;(declare-fun a () String)
;(declare-fun index () Int)
(declare-fun sli () String)




;(assert (= parameter "eOfferCode") )
;(assert (= index (str.indexof a "eOfferCode" 0) ) )

;(assert (= index (- 0 1) ) )
(assert (= sli "") )

(assert (not (= sli "") ) )


(check-sat)
(get-model)
