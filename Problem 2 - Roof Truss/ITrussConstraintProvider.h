#ifndef C_EXERCISE_ITRUSSCONSTRAINTPROVIDER_H
#define C_EXERCISE_ITRUSSCONSTRAINTPROVIDER_H

#include "TrussModels.h"

/**
 * Interface for providers that generate TrussConstraints based on a given TrussInput.
 * This allows for different truss designs and constraint generation logic to be implemented (once they exist).
 */
class ITrussConstraintProvider {
public:
    virtual ~ITrussConstraintProvider() = default;
    virtual TrussConstraints build(const TrussInput& input) const = 0;
};

#endif
