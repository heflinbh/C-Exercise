#ifndef C_EXERCISE_BAY_H
#define C_EXERCISE_BAY_H

class Bay {
public:
    struct JoinWindow {
        double startMm;
        double endMm;
    };

    Bay(int index, double startMm, double bayLengthMm);

    int getIndex() const;
    double getStartMm() const;
    double getEndMm() const;
    double getLengthMm() const;

    JoinWindow getLowerJoinRange() const; // 10% to 25%
    JoinWindow getUpperJoinRange() const;  // 75% to 90%

private:
    int index;
    double startMm;
    double bayLengthMm;

    static constexpr double lowerJoinStartPercent = 0.10;
    static constexpr double lowerJoinEndPercent = 0.25;
    static constexpr double upperJoinStartPercent = 0.75;
    static constexpr double upperJoinEndPercent = 0.90;
};

#endif // C_EXERCISE_BAY_H
