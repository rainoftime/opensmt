//
// Created by Martin Blicha on 06.11.20.
//

#ifndef OPENSMT_LIAINTERPOLATOR_H
#define OPENSMT_LIAINTERPOLATOR_H

#include "FarkasInterpolator.h"

class LIAInterpolator {

    std::unique_ptr<FarkasInterpolator> farkasInterpolator;

public:
    LIAInterpolator(LALogic & logic, vec<PtAsgn> const & explanations, std::vector<opensmt::Real> const & coeffs,
    std::map<PTRef, icolor_t> & labels);


    PTRef getFarkasInterpolant() { return farkasInterpolator->getFarkasInterpolant(); }
    PTRef getDualFarkasInterpolant() { return farkasInterpolator->getDualFarkasInterpolant(); }
    PTRef getFlexibleInterpolant(opensmt::Real alpha) { return farkasInterpolator->getFlexibleInterpolant(std::move(alpha)); }
    PTRef getDecomposedInterpolant() { return farkasInterpolator->getDecomposedInterpolant(); }
    PTRef getDualDecomposedInterpolant() { return farkasInterpolator->getDualFarkasInterpolant(); }
};


#endif //OPENSMT_LIAINTERPOLATOR_H
