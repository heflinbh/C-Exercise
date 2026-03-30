#include "BenCustomTrussConstraintProvider.h"

#include <cmath>
#include <limits>

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

double BenCustomTrussConstraintProvider::getLengthOfDiagonalMm(double trussSpanMm, double roofAngleRadians) {
    const double halfSpanMm = trussSpanMm / 2.0;
    const double cosTheta = std::cos(roofAngleRadians);

    if (std::abs(cosTheta) < 1e-12) {
        return std::numeric_limits<double>::infinity();
    }

    return halfSpanMm / cosTheta;
}

double BenCustomTrussConstraintProvider::getBayLengthOfBottomMm(double bottomLengthMm) {
    return bottomLengthMm / numBaysBottom;
}

double BenCustomTrussConstraintProvider::getBayLengthOfDiagonalMm(double diagonalLengthMm) {
    return diagonalLengthMm / numBaysDiagonal;
}

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
