(set-logic QF_LRA)
(declare-fun v4 () Bool)
(declare-fun i0 () Real)
(assert (< i0 71))
(check-sat)
(assert (xor (and (< i0 71) v4) v4))
(check-sat)
(check-sat)
