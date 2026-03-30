#ifndef C_EXERCISE_TRUSSTYPES_H
#define C_EXERCISE_TRUSSTYPES_H

#include <vector>

struct TrussInput {
    int spanMm;
    double roofAngleRad;
};

struct JoinZone {
    double startMm;
    double endMm;
};

struct MemberConstraints {
    double memberLengthMm;
    double bayLengthMm;
    int bayCount;
    std::vector<JoinZone> joinZones;
};

struct TrussConstraints {
    MemberConstraints diagonalTruss; // one side
    MemberConstraints bottomTruss;
};

struct CutPiece {
    double lengthOfPieceMm;
    int originalStockLengthMm;
    double wasteMm;
};

struct MemberCutPlan {
    std::vector<CutPiece> pieces;
    double totalWasteMm = 0.0;
};

struct CutPlan {
    MemberCutPlan diagonalLeft;
    MemberCutPlan diagonalRight;
    MemberCutPlan bottom;
    double totalWasteMm = 0.0;
};

#endif
