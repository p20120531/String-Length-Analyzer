(set-logic QF_S)
(set-option :produce-models true)


;(declare-fun src () String)
(declare-fun URL () String)
;(declare-fun ret () String)
;(declare-fun pred () Bool)
;(declare-fun p1 () Bool)
(declare-fun i1 () Int)
(declare-fun i2 () Int)


;(assert (= p1 (str.suffixof  "_hA"  src) ) )
;(assert (= pred  (or (str.suffixof  "_ha"  src) (str.suffixof "_Ha"  src) (str.suffixof "_HA"  src) p1 ) ) )


;(assert (= src "") )
;(assert (not (> (str.indexof URL "www.ha45.com/index.html" 0) (- 0 1) ) ) )
(assert (not (> i1 (- 0 1))))
(assert (= i1 (str.indexof URL "www.ha45.com/index.html" 0)))
(assert (str.contains URL "www.ha45.com/index.html") ) ; for indexof

;(assert (> (str.indexof URL "www.ha45.com/index.htm" 0) (- 0 1) ) )
(assert (> i2 (- 0 1)))
(assert (= i2 (str.indexof URL "www.ha45.com/index.htm" 0)))
(assert (str.contains URL "www.ha45.com/index.htm") ) ; for indexof

;(assert (= ret "srcieak_ha") )


;(assert (str.suffixof "_ha" ret) )
;(assert (not (= src "") ) )

(check-sat)
(get-model)