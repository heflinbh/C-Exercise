#include "BenCustomTrussConstraintProvider.h"

#include <cmath>
#include <limits>

/**
 * Takes TrussInput and calculates the lengths of the diagonal and bottom members, the bay lengths, and the join zones for both member types.
 * It then constructs and returns a TrussConstraints object containing all this information.
 *
 * Assumes a truss with one bottom length and two diagonal lengths.
 */
TrussConstraints BenCustomTrussConstraintProvider::build(const TrussInput& input) const {
    const double bottomLengthMm = input.spanMm;
    const double diagonalLengthMm = getLengthOfDiagonalMm(input.spanMm, input.roofAngleRad);

    const double bottomBayLengthMm = getBayLengthOfBottomMm(bottomLengthMm);
    const double diagonalBayLengthMm = getBayLengthOfDiagonalMm(diagonalLengthMm);

    const std::vector<JoinZone> diagonalJoinZones = getJoinZones(numBaysDiagonal, diagonalBayLengthMm);
    const std::vector<JoinZone> bottomJoinZones = getJoinZones(numBaysBottom, bottomBayLengthMm);

    const MemberConstraints diagonalConstraints {
        diagonalLengthMm,
        diagonalBayLengthMm,
        numBaysDiagonal,
        diagonalJoinZones};

    const MemberConstraints bottomConstraints {
        bottomLengthMm,
        bottomBayLengthMm,
        numBaysBottom,
        bottomJoinZones};

    return TrussConstraints {
        diagonalConstraints,
        bottomConstraints};
}

/**
 * Calculates the length of the diagonal member using trigonometry.
 * The diagonal forms the hypotenuse of a right triangle where half the span is one leg and the roof angle determines the relationship between the legs.
 */
double BenCustomTrussConstraintProvider::getLengthOfDiagonalMm(double trussSpanMm, double roofAngleRadians) {
    const double halfSpanMm = trussSpanMm / 2.0;
    const double cosTheta = std::cos(roofAngleRadians);

    if (std::abs(cosTheta) < 1e-12) {
        return std::numeric_limits<double>::infinity();
    }

    return halfSpanMm / cosTheta;
}

/**
 * Calculates the bay length for the bottom member by dividing the total bottom length by the number of bays.
 */
double BenCustomTrussConstraintProvider::getBayLengthOfBottomMm(double bottomLengthMm) {
    return bottomLengthMm / numBaysBottom;
}

/**
 * Calculates the bay length for the diagonal members by dividing the total diagonal length by the number of bays.
 */
double BenCustomTrussConstraintProvider::getBayLengthOfDiagonalMm(double diagonalLengthMm) {
    return diagonalLengthMm / numBaysDiagonal;
}

/**
 * Generates a vector of JoinZone objects for the specified number of bays and bay length.
 * Each bay has two join zones defined by the lower and upper join start and end percentages.
 * The join zones are calculated as absolute positions along the member length based on the bay length and the specified percentages.
 */
std::vector<JoinZone> BenCustomTrussConstraintProvider::getJoinZones(int numBays, double bayLengthMm) {
    std::vector<JoinZone> zones;
    zones.reserve(numBays * 2);

    for (int bay = 0; bay < numBays; ++bay) {
        const double bayStartMm = bay * bayLengthMm;

        zones.push_back(JoinZone {
            bayStartMm + bayLengthMm * lowerJoinStart,
            bayStartMm + bayLengthMm * lowerJoinEnd});

        zones.push_back(JoinZone {
            bayStartMm + bayLengthMm * upperJoinStart,
            bayStartMm + bayLengthMm * upperJoinEnd});
    }

    return zones;
}
