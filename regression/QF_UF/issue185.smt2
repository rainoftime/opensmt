(set-logic QF_UF)
(declare-fun f (Bool) Bool)
(declare-fun v7 () Bool)
(declare-fun v8 () Bool)
(assert (f (= v7 v8)))
(check-sat)
(exit)
