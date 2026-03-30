#include "GreedySingleMemberSolver.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

GreedySingleMemberSolver::GreedySingleMemberSolver(const MemberConstraints& member, const std::vector<int>& providedStockLengthsMm) {
    this->validatedStockLengthsMm = validateStockLengths(providedStockLengthsMm);
    if (validatedStockLengthsMm.empty()) {
        throw std::invalid_argument("No valid stock lengths provided.");
    }

    if (member.memberLengthMm <= 0) {
        throw std::invalid_argument("Member required length must be greater than zero.");
    }
    this->member = member;
}

SingleMemberSolution GreedySingleMemberSolver::solve() const {
    SingleMemberSolution solution;
    double builtToMm = 0.0;

    while (!memberIsComplete(builtToMm)) {
        bool foundCandidate = false;
        int bestStockLengthMm = 0;
        double bestNextBuiltToMm = 0.0;
        double bestRequiredLengthMm = 0.0;
        double bestWasteMm = 0.0;

        for (int stockLengthMm : validatedStockLengthsMm) {
            const std::optional<double> candidateBuiltTo =
                findFurthestLegalEndpoint(builtToMm, stockLengthMm);

            if (!candidateBuiltTo.has_value()) {
                continue;
            }

            const double requiredLengthMm = candidateBuiltTo.value() - builtToMm;
            const double wasteMm = static_cast<double>(stockLengthMm) - requiredLengthMm;

            if (isBetterCandidate(foundCandidate, wasteMm, bestWasteMm, candidateBuiltTo.value(), bestNextBuiltToMm, stockLengthMm, bestStockLengthMm)) {
                foundCandidate = true;
                bestStockLengthMm = stockLengthMm;
                bestNextBuiltToMm = candidateBuiltTo.value();
                bestRequiredLengthMm = requiredLengthMm;
                bestWasteMm = wasteMm;}
        }

        if (!foundCandidate) {
            throw std::runtime_error("No valid next cut found. Cannot complete member with given stock lengths.");
        }

        solution.cutPlan.pieces.push_back(CutPiece {
            bestRequiredLengthMm,
            bestStockLengthMm,
            bestWasteMm});

        solution.cutPlan.totalWasteMm += bestWasteMm;
        solution.stockUsageCounts[bestStockLengthMm] += 1;

        builtToMm = bestNextBuiltToMm;
    }

    return solution;
}

bool GreedySingleMemberSolver::memberIsComplete(double builtToMm) const {
    return builtToMm >= member.memberLengthMm;
}

std::optional<double> GreedySingleMemberSolver::findFurthestLegalEndpoint(double builtToMm, int stockLengthMm) const {
    constexpr double joinEndClearanceMm = 10.0;

    if (stockLengthMm <= 0) {
        return std::nullopt;
    }

    const double maxReachMm = std::min(member.memberLengthMm, builtToMm + static_cast<double>(stockLengthMm));
    if (maxReachMm <= builtToMm) {
        return std::nullopt;
    }

    std::optional<double> furthest;

    // End of member is always legal if reachable (no clearance rule here).
    if (member.memberLengthMm <= maxReachMm) {
        furthest = member.memberLengthMm;
    }

    // For join zones, keep 10 mm available for the subsequent piece.
    for (const JoinZone& zone : member.joinZones) {
        if (zone.endMm <= builtToMm) {
            continue;
        }
        if (zone.startMm > maxReachMm) {
            continue;
        }

        const double zoneUsableEndMm = zone.endMm - joinEndClearanceMm;
        if (zoneUsableEndMm < zone.startMm) {
            continue; // zone too small after applying clearance
        }

        const double candidate = std::min(maxReachMm, zoneUsableEndMm);
        if (candidate <= builtToMm) {
            continue;
        }

        if (!furthest.has_value() || candidate > furthest.value()) {
            furthest = candidate;
        }
    }

    return furthest;
}

bool GreedySingleMemberSolver::isBetterCandidate(
    bool foundCandidate,
    double wasteMm,
    double bestWasteMm,
    double candidateBuiltToMm,
    double bestNextBuiltToMm,
    int stockLengthMm,
    int bestStockLengthMm) {

    if (!foundCandidate) return true;

    if (wasteMm != bestWasteMm) {
        return wasteMm < bestWasteMm;
    }

    if (candidateBuiltToMm != bestNextBuiltToMm) {
        return candidateBuiltToMm > bestNextBuiltToMm;
    }

    return stockLengthMm < bestStockLengthMm;
}


/**
 * Helper method to filter out any provided stock lengths that make no sense, i.e. those with negative lengths.
 */
std::vector<int> GreedySingleMemberSolver::validateStockLengths(const std::vector<int>& providedStockLengthsMm) {
    std::vector<int> validatedStockLengths;

    for (int length : providedStockLengthsMm) {
        if (length > 0) {
            validatedStockLengths.push_back(length);
        }
    }

    std::sort(validatedStockLengths.begin(), validatedStockLengths.end());
    validatedStockLengths.erase(std::unique(validatedStockLengths.begin(), validatedStockLengths.end()), validatedStockLengths.end());

    return validatedStockLengths;
}

