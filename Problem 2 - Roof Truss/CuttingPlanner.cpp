#include "CuttingPlanner.h"
#include "GreedySingleMemberSolver.h"
#include <utility>

/**
 * Responsible for generating a cutting plan for all truss members based on the provided stock lengths and the constraints of the truss design.
 * In the future, this class can be updated to use other types of ISingleMemberSolver. For now, it only considers the GreedySingleMemberSolver implementation.
 */
CuttingPlanner::CuttingPlanner(std::vector<int> providedStockLengthsMm)
    : providedStockLengthsMm(std::move(providedStockLengthsMm)) {
}

/**
 * Generates a cutting plan for the bottom and diagonal members of the truss using the algorithm from GreedySingleMemberSolver.
 * Because of its use of GreedySingleMemberSolver, it safely assumes that diagonalLeft and diagonalRight can use identical data.
 */
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
