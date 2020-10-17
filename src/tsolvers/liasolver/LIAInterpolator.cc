//
// Created by Martin Blicha on 06.11.20.
//

#include "LIAInterpolator.h"
#include "LALogic.h"

#include <memory>

LIAInterpolator::LIAInterpolator(LALogic & logic, const vec<PtAsgn> & explanations,
                                 const vector<opensmt::Real> & coeffs, map<PTRef, icolor_t> & labels) {
    vec<PtAsgn> liaExplanation;
    // We need to recompute the labels for the Farkas interpolator!
    // Consider this example: not (0 <= x - y) with label A; not (1 <= y - x) with label B
    // After strengthening we get 1 <= y - x with label A; 0 <= x - y with label B
    // The labels have been switched for the positive atoms, and we need to make sure that interpolator sees the second version of the labels
    std::map<PTRef, icolor_t> newLabels;
    for (auto explEntry : explanations) {
        PTRef positiveInequality = explEntry.tr;
        lbool sign = explEntry.sgn;
        assert(logic.isNumLeq(positiveInequality));
        assert(sign == l_True or sign == l_False);
        PTRef boundVal = logic.getConstantFromLeq(positiveInequality);
        PTRef boundedTerm = logic.getTermFromLeq(positiveInequality);
        assert(logic.isNumConst(boundVal) and logic.isLinearTerm(boundedTerm));
        assert(logic.getNumConst(boundVal).isInteger()); // We assume LIA inequalities are already strengthened
        if (sign == l_True) {
            // Already strengthened!
            liaExplanation.push(PtAsgn(positiveInequality, l_True));
        } else {
            // 'not (c <= term)' => 'c > term' => 'term < c' => 'term <= c-1' => -(c-1) <= -term
            auto newBoundValue = (logic.getNumConst(boundVal) - 1);
            newBoundValue.negate();
            PTRef nInequality = logic.mkNumLeq(logic.mkConst(newBoundValue), logic.mkNumNeg(boundedTerm));
            assert(logic.getTermFromLeq(nInequality) == logic.mkNumNeg(boundedTerm));
            liaExplanation.push(PtAsgn(nInequality, l_True));
        }
        // Ensure the strengthened inequality has correct partition information
        PTRef nInequality = liaExplanation.last().tr;
        auto res = newLabels.insert(std::make_pair(nInequality, labels.at(positiveInequality)));
        if (not res.second) {
            // key has been already present
            throw OsmtInternalException("Error in preparing LIA interpolation");
        }
    }
    farkasInterpolator = std::unique_ptr<FarkasInterpolator>(
        new FarkasInterpolator(logic, std::move(liaExplanation), std::move(coeffs), std::move(newLabels)));
}
