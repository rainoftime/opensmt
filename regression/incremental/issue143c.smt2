(set-logic QF_LRA)
(declare-fun v0 () Bool)
(declare-fun v1 () Bool)
(declare-fun v2 () Bool)
(check-sat)
(check-sat)
(assert (xor v1 v0))
(check-sat)
(check-sat)
(assert (= (distinct v0 v2) v1))
(push 1)
(check-sat)
(check-sat)
(pop 1)
(check-sat)
(assert false)
(check-sat)
