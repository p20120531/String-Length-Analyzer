(set-logic QF_S)

(declare-fun T_1 () Bool)
(declare-fun T_10 () Bool)
(declare-fun T_11 () Bool)
(declare-fun T_12 () Bool)
(declare-fun T_13 () Bool)
(declare-fun T_14 () Bool)
(declare-fun T_15 () Bool)
(declare-fun T_16 () Bool)
(declare-fun T_17 () Bool)
(declare-fun T_18 () Bool)
(declare-fun T_19 () Bool)
(declare-fun T_1a () Bool)
(declare-fun T_1b () Bool)
(declare-fun T_1c () Bool)
(declare-fun T_1d () Bool)
(declare-fun T_1e () Bool)
(declare-fun T_1f () Bool)
(declare-fun T_2 () Bool)
(declare-fun T_20 () Bool)
(declare-fun T_21 () Bool)
(declare-fun T_22 () Bool)
(declare-fun T_23 () Bool)
(declare-fun T_3 () Bool)
(declare-fun T_4 () Bool)
(declare-fun T_5 () Bool)
(declare-fun T_6 () Bool)
(declare-fun T_7 () Bool)
(declare-fun T_8 () Bool)
(declare-fun T_9 () Bool)
(declare-fun T_a () Bool)
(declare-fun T_b () Bool)
(declare-fun T_c () Bool)
(declare-fun T_d () Bool)
(declare-fun T_e () Bool)
(declare-fun T_f () Bool)
(declare-fun var_0xINPUT_84997 () String)

(assert (= T_1 (not (= "" var_0xINPUT_84997))))
(assert T_1)
(assert (= T_2 (not (= "" var_0xINPUT_84997))))
(assert T_2)
(assert (= T_3 (not (= "" var_0xINPUT_84997))))
(assert T_3)
(assert (= T_4 (not (= var_0xINPUT_84997 ""))))
(assert T_4)
(assert (= T_5 (= var_0xINPUT_84997 "")))
(assert (= T_6 (not T_5)))
(assert T_6)
(assert (= T_7 (not (= "" var_0xINPUT_84997))))
(assert T_7)
(assert (= T_8 (not (= "6JX7G3VKFq" var_0xINPUT_84997))))
(assert T_8)
(assert (= T_9 (not (= var_0xINPUT_84997 "6JX7G3VKFq"))))
(assert T_9)
(assert (= T_a (= var_0xINPUT_84997 "")))
(assert (= T_b (not T_a)))
(assert T_b)
(assert (= T_c (not (= "" var_0xINPUT_84997))))
(assert T_c)
(assert (= T_d (not (= "" var_0xINPUT_84997))))
(assert T_d)
(assert (= T_e (not (= var_0xINPUT_84997 ""))))
(assert T_e)
(assert (= T_f (= var_0xINPUT_84997 "")))
(assert (= T_10 (not T_f)))
(assert T_10)
(assert (= T_11 (not (= "" var_0xINPUT_84997))))
(assert T_11)
(assert (= T_12 (not (= "" var_0xINPUT_84997))))
(assert T_12)
(assert (= T_13 (not (= var_0xINPUT_84997 ""))))
(assert T_13)
(assert (= T_14 (= var_0xINPUT_84997 "")))
(assert (= T_15 (not T_14)))
(assert T_15)
(assert (= T_16 (not (= "" var_0xINPUT_84997))))
(assert T_16)
(assert (= T_17 (= var_0xINPUT_84997 "Example:")))
(assert (= T_18 (not T_17)))
(assert T_18)
(assert (= T_19 (not (= "" var_0xINPUT_84997))))
(assert T_19)
(assert (= T_1a (not (= var_0xINPUT_84997 "fB9De3KAY9"))))
(assert T_1a)
(assert (= T_1b (not (= "fB9De3KAY9" var_0xINPUT_84997))))
(assert T_1b)
(assert (= T_1c (= var_0xINPUT_84997 "")))
(assert (= T_1d (not T_1c)))
(assert T_1d)
(assert (= T_1e (not (= "" var_0xINPUT_84997))))
(assert T_1e)
(assert (= T_1f (not (= "6JX7G3VKFq" var_0xINPUT_84997))))
(assert T_1f)
(assert (= T_20 (not (= var_0xINPUT_84997 "6JX7G3VKFq"))))
(assert T_20)
(assert (= T_21 (= var_0xINPUT_84997 "")))
(assert (= T_22 (not T_21)))
(assert T_22)
(assert (= T_23 (not (= var_0xINPUT_84997 ""))))
(assert T_23)

(check-sat)

