#ifndef C_EXERCISE_GREEDYSINGLEMEMBERSOLVER_H
#define C_EXERCISE_GREEDYSINGLEMEMBERSOLVER_H

#include "ISingleMemberSolver.h"
#include <map>
#include <vector>
#include <optional>

class GreedySingleMemberSolver : public ISingleMemberSolver {
public:
    GreedySingleMemberSolver(const MemberConstraints& member, const std::vector<int>& providedStockLengthsMm);

    SingleMemberSolution solve() const override;

private:
    MemberConstraints memberConstraints;
    std::vector<int> validatedStockLengthsMm;

    static std::vector<int> validateStockLengths(const std::vector<int>& providedStockLengthsMm);

    bool memberIsComplete(double builtToMm) const;
    std::optional<double> findFurthestLegalEndpoint(double builtToMm, int stockLengthMm) const;
    static bool isBetterCandidate(bool foundCandidate,
                                  double wasteMm,
                                  double bestWasteMm,
                                  double candidateBuiltToMm,
                                  double bestNextBuiltToMm,
                                  int stockLengthMm,
                                  int bestStockLengthMm);
};

#endif // C_EXERCISE_GREEDYSINGLEMEMBERSOLVER_H
