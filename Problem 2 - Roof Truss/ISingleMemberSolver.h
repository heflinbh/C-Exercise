#ifndef C_EXERCISE_ISINGLEMEMBERSOLVER_H
#define C_EXERCISE_ISINGLEMEMBERSOLVER_H

#include "TrussModels.h"
#include <map>

struct SingleMemberSolution {
    MemberCutPlan cutPlan;
    std::map<int, int> stockUsageCounts;
};

class ISingleMemberSolver {
public:
    virtual ~ISingleMemberSolver() = default;
    virtual SingleMemberSolution solve() const = 0;
};

#endif // C_EXERCISE_ISINGLEMEMBERSOLVER_H
