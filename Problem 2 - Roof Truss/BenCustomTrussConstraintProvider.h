#ifndef C_EXERCISE_BENCUSTOMTRUSSCONSTRAINTPROVIDER_H
#define C_EXERCISE_BENCUSTOMTRUSSCONSTRAINTPROVIDER_H


#include "ITrussConstraintProvider.h"
#include <vector>

class BenCustomTrussConstraintProvider : public ITrussConstraintProvider {
public:
    TrussConstraints build(const TrussInput& input) const override;

private:
    static constexpr int numBaysBottom = 5;
    static constexpr int numBaysDiagonal = 3;

    static constexpr double lowerJoinStart = 0.10;
    static constexpr double lowerJoinEnd = 0.25;
    static constexpr double upperJoinStart = 0.75;
    static constexpr double upperJoinEnd = 0.90;

    static double getLengthOfDiagonalMm(double trussSpanMm, double roofAngleRadians);
    static double getBayLengthOfBottomMm(double bottomLengthMm);
    static double getBayLengthOfDiagonalMm(double diagonalLengthMm);
    static std::vector<JoinZone> getJoinZones(int numBays, double bayLengthMm);
};

#endif
