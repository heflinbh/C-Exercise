#include "Bay.h"

Bay::Bay(int index, double startMm, double bayLengthMm)
    : index(index), startMm(startMm), bayLengthMm(bayLengthMm) {
}

int Bay::getIndex() const {
    return index;
}

double Bay::getStartMm() const {
    return startMm;
}

double Bay::getEndMm() const {
    return startMm + bayLengthMm;
}

double Bay::getLengthMm() const {
    return bayLengthMm;
}

Bay::JoinWindow Bay::getLowerJoinRange() const {
    return {
        startMm + bayLengthMm * lowerJoinStartPercent,
        startMm + bayLengthMm * lowerJoinEndPercent
    };
}

Bay::JoinWindow Bay::getUpperJoinRange() const {
    return {
        startMm + bayLengthMm * upperJoinStartPercent,
        startMm + bayLengthMm * upperJoinEndPercent
    };
}
