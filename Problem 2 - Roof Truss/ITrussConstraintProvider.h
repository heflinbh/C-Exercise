#ifndef C_EXERCISE_ITRUSSCONSTRAINTPROVIDER_H
#define C_EXERCISE_ITRUSSCONSTRAINTPROVIDER_H

#include "TrussModels.h"

class ITrussConstraintProvider {
public:
    virtual ~ITrussConstraintProvider() = default;
    virtual TrussConstraints build(const TrussInput& input) const = 0;
};

#endif
