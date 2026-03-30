#include "CuttingPlanner.h"

#include "GreedySingleMemberSolver.h"

CuttingPlanner::CuttingPlanner(std::vector<int> providedStockLengthsMm)
    : providedStockLengthsMm(std::move(providedStockLengthsMm)) {
}

CutPlan CuttingPlanner::planGreedy(const TrussConstraints& constraints) const {
    CutPlan plan;

    GreedySingleMemberSolver bottomSolver(constraints.bottomTruss, providedStockLengthsMm);
    GreedySingleMemberSolver diagonalSolver(constraints.diagonalTruss, providedStockLengthsMm);
    const SingleMemberSolution bottomSolution = bottomSolver.solve();
    const SingleMemberSolution diagonalSolution = diagonalSolver.solve();

    plan.bottom = bottomSolution.cutPlan;
    plan.diagonalLeft = diagonalSolution.cutPlan;
    plan.diagonalRight = diagonalSolution.cutPlan;

    plan.totalWasteMm = bottomSolution.cutPlan.totalWasteMm + (2 * diagonalSolution.cutPlan.totalWasteMm);

    return plan;
}
