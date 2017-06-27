(set-logic QF_S)
(set-option :produce-models true)

(declare-fun s () String)


(assert (str.contains s ">") )
(assert (str.contains s "sc") )
(assert (str.contains s "ript scr=") )


(check-sat)
(get-model)
