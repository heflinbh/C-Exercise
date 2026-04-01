#include "GreedySingleMemberSolver.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

/**
 * Produces a cutting solution for a single member of a truss using a greedy algorithm.
 */
GreedySingleMemberSolver::GreedySingleMemberSolver(const MemberConstraints& member, const std::vector<int>& providedStockLengthsMm)
    : memberConstraints(member), validatedStockLengthsMm(validateStockLengths(providedStockLengthsMm)) {

    if (validatedStockLengthsMm.empty()) {
        throw std::invalid_argument("No valid stock lengths provided.");
    }

    if (member.memberLengthMm <= 0) {
        throw std::invalid_argument("Member required length must be greater than zero.");
    }
}

/**
 * Iteratively builds the member from the start (0 mm) to the end (member.memberLengthMm) by repeatedly selecting the best next cut based on the provided stock lengths and the member's join zones.
 */
SingleMemberSolution GreedySingleMemberSolver::solve() const {
    SingleMemberSolution solution;
    double builtToMm = 0.0;

    // Do this same thing until the member is completely solved.
    while (!memberIsComplete(builtToMm)) {
        bool foundCandidate = false;
        int bestStockLengthMm = 0;
        double bestNextBuiltToMm = 0.0;
        double bestRequiredLengthMm = 0.0;
        double bestWasteMm = 0.0;

        // For each stock length, find the furthest legal endpoint that can be reached from the current builtToMm.
        // We then compare all candidates to find the best one based on "some criteria", which we define in isBetterCandidate().
        for (int stockLengthMm : validatedStockLengthsMm) {

            // Check whether this stock length can make a valid next cut. If not, move on.
            const std::optional<double> candidateBuiltTo = findFurthestLegalEndpoint(builtToMm, stockLengthMm);
            if (!candidateBuiltTo.has_value()) {
                continue;
            }

            const double requiredLengthMm = candidateBuiltTo.value() - builtToMm;
            const double wasteMm = static_cast<double>(stockLengthMm) - requiredLengthMm;

            // We have a valid candidate cut, now check if it's better than the best found so far.
            if (isBetterCandidate(foundCandidate, wasteMm, bestWasteMm, candidateBuiltTo.value(), bestNextBuiltToMm, stockLengthMm, bestStockLengthMm)) {
                foundCandidate = true;
                bestStockLengthMm = stockLengthMm;
                bestNextBuiltToMm = candidateBuiltTo.value();
                bestRequiredLengthMm = requiredLengthMm;
                bestWasteMm = wasteMm;
            }
        }

        // This is where our code breaks, if it turns out there is no solution for a given geometry.
        if (!foundCandidate) {
            throw std::runtime_error("No valid next cut found. Cannot complete member with given stock lengths.");
        }

        // Record the best cut found in the solution.
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

/**
 * Determines if the member is complete based on how far along it has been built (builtToMm) compared to the total required length of the member.
 */
bool GreedySingleMemberSolver::memberIsComplete(double builtToMm) const {
    return builtToMm >= memberConstraints.memberLengthMm;
}

/**
 * Finds the furthest legal endpoint along the member that can be reached from the current builtToMm using a stock piece of the given length.
 *
 * NOTE: This method assumes a "join clearance" of 10mm. This is to ensure that any subsequent joins have enough space to, well, join.
 *
 * Returns std::nullopt if no legal endpoint can be reached with the given stock length. This signifies that no solution is possible from the GreedySingleMemberSolver.
 */
std::optional<double> GreedySingleMemberSolver::findFurthestLegalEndpoint(double builtToMm, int stockLengthMm) const {
    constexpr double joinEndClearanceMm = 10.0;

    if (stockLengthMm <= 0) {
        return std::nullopt;
    }

    const double maxReachMm = std::min(memberConstraints.memberLengthMm, builtToMm + static_cast<double>(stockLengthMm));
    if (maxReachMm <= builtToMm) {
        return std::nullopt;
    }

    std::optional<double> furthest;

    // End of member is always legal if reachable (no clearance rule here).
    if (memberConstraints.memberLengthMm <= maxReachMm) {
        furthest = memberConstraints.memberLengthMm;
    }

    // For join zones, keep 10 mm available for the subsequent piece.
    for (const JoinZone& zone : memberConstraints.joinZones) {
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

/**
 * Determines if a candidate cut is better than the best found so far.
 * Makes decisions based on the following criteria, in order of importance:
 * 1. Lower waste is always better.
 * 2. If waste is the same, a cut that builds further along the member is better.
 * 3. If both waste and built-to point are the same, a cut using a shorter stock length is better (to preserve longer
 * stock for later cuts).
 */
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
