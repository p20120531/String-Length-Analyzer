(set-logic QF_S)
(set-option :produce-models true)


(declare-fun selKeyword_value_trimed () String)
(declare-fun p2 () String)
(declare-fun url () String)
(declare-fun ret () String)
(declare-fun houseCityKeyName () String)


(assert (not (str.suffixof " " p2) ) )

(assert (not (str.suffixof "\t" p2) ) )

(assert (not (str.prefixof "\t" p2) ) )

(assert (not (str.prefixof " " p2) ) )

(assert (= selKeyword_value_trimed p2) )

(assert (= url (str.++  "http://xf.house.somesite.com/"  houseCityKeyName  "/search/0-0-0-0-0-0-0-0-0-1-1.html") ) )

(assert (not (= "" selKeyword_value_trimed ) ) )
(assert (= ret (str.++  url  "?keyword="  selKeyword_value_trimed) ) )


(check-sat)
(get-model)

