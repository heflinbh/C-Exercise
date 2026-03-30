#ifndef C_EXERCISE_ISINGLEMEMBERSOLVER_H
#define C_EXERCISE_ISINGLEMEMBERSOLVER_H

#include "TrussModels.h"
#include <map>

struct SingleMemberSolution {
    MemberCutPlan cutPlan;
    std::map<int, int> stockUsageCounts;
};

/**
 * Interface for solvers that generate cutting plans for a single member of a truss.
 * This allows for different algorithms to be implemented and used interchangeably in the CuttingPlanner (once they exist).
 */
class ISingleMemberSolver {
public:
    virtual ~ISingleMemberSolver() = default;
    virtual SingleMemberSolution solve() const = 0;
};

#endif // C_EXERCISE_ISINGLEMEMBERSOLVER_H
