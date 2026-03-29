#include "GeometryCalculator.h"

#include <cmath>    // std::cos
#include <limits>   // std::numeric_limits

GeometryCalculator::GeometryCalculator(int trussSpanMm, double roofAngleDegrees)
    : trussSpanMm(trussSpanMm), roofAngleDegrees(roofAngleDegrees) {
}

double GeometryCalculator::getLengthOfDiagonalMm() const {
    const double halfSpanMm = static_cast<double>(trussSpanMm) / 2.0;
    const double angleRad = roofAngleDegrees * pi / 180.0;
    const double cosTheta = std::cos(angleRad);

    // Guard against near-vertical angles to avoid huge/undefined results.
    if (std::abs(cosTheta) < 1e-12) {
        return std::numeric_limits<double>::infinity();
    }

    return halfSpanMm / cosTheta;
}

double GeometryCalculator::getLengthOfBottomMm() const {
    return trussSpanMm;
}

double GeometryCalculator::getBayLengthOfDiagonalMm() const {
    return getLengthOfDiagonalMm() / numBaysDiagonal;
}

double GeometryCalculator::getBayLengthOfBottomMm() const {
    return static_cast<double>(trussSpanMm) / numBaysBottom;
}

