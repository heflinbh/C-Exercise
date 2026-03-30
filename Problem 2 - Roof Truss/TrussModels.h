#ifndef C_EXERCISE_TRUSSINPUT_H
#define C_EXERCISE_TRUSSINPUT_H

#include <vector>

/**
 * The input from the user.
 * This must include all necessary information to fully describe the truss.
 */
struct TrussInput {
    int spanMm;
    double roofAngleRad;
};

/**
 * Describes a zone in which joins are possible in a single member.
 */
struct JoinZone {
    double startMm;
    double endMm;
};

/**
 * Describes a single member of a truss, including bay definition and join zones.
 */
struct MemberConstraints {
    double memberLengthMm;
    double bayLengthMm;
    int bayCount;
    std::vector<JoinZone> joinZones;
};

/**
 * Describes the constraints for all members of the truss. In this case, we have one bottom member and two diagonal members (which share the same constraints).
 * This structure will need modification if alternate truss designs are introduced.
 */
struct TrussConstraints {
    MemberConstraints diagonalTruss; // describes one side only!
    MemberConstraints bottomTruss;
};

/**
 * Represents a single cut piece of stock, including the length of the piece, the original stock length it was cut from, and the waste generated from that cut.
 */
struct CutPiece {
    double lengthOfPieceMm;
    int originalStockLengthMm;
    double wasteMm;
};

/**
 * A plan for cutting a single member of the truss, consisting of the pieces to cut and the total waste generated from those cuts.
 */
struct MemberCutPlan {
    std::vector<CutPiece> pieces;
    double totalWasteMm = 0.0;
};

/**
 * A plan for cutting all members of the truss, consisting of the cut plans for the bottom and diagonal members, as well as the total waste generated from cutting all members.
 * As with TrussConstraints, this structure will need modification if alternate truss designs are introduced.
 */
struct CutPlan {
    MemberCutPlan diagonalLeft;
    MemberCutPlan diagonalRight;
    MemberCutPlan bottom;
    double totalWasteMm = 0.0;
};

#endif
