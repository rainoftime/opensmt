//
// Created by Martin Blicha on 29.07.20.
//

#ifndef OPENSMT_VERIFICATIONUTILS_H
#define OPENSMT_VERIFICATIONUTILS_H

#include "SMTConfig.h"
#include "Logic.h"

class VerificationUtils {
    SMTConfig const & config;
    Logic & logic;
public:
    VerificationUtils(SMTConfig const & config, Logic & logic) : config(config), logic(logic) {}

    bool implies(PTRef, PTRef); // Check the result with an external solver

    bool verifyInterpolant(PTRef, const ipartitions_t &);

private:
    bool verifyInterpolantA(PTRef, const ipartitions_t &);

    bool verifyInterpolantB(PTRef, const ipartitions_t &);
};



#endif //OPENSMT_VERIFICATIONUTILS_H
