#ifndef C_EXERCISE_CUTTINGPLANNER_H
#define C_EXERCISE_CUTTINGPLANNER_H

#include "TrussModels.h"
#include <map>
#include <vector>

class CuttingPlanner {
public:
    explicit CuttingPlanner(std::vector<int> providedStockLengthsMm);

    CutPlan planGreedy(const TrussConstraints& constraints) const;

private:
    std::vector<int> providedStockLengthsMm;
};

#endif // C_EXERCISE_CUTTINGPLANNER_H
