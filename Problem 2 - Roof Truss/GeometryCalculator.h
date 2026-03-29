#ifndef C_EXERCISE_TRUSSCALCULATOR_H
#define C_EXERCISE_TRUSSCALCULATOR_H

class GeometryCalculator {
public:
    GeometryCalculator(int trussSpanMm, double roofAngleDegrees);

    double getLengthOfDiagonalMm() const;
    double getLengthOfBottomMm() const;
    double getBayLengthOfDiagonalMm() const;
    double getBayLengthOfBottomMm() const;

private:
    const int trussSpanMm;
    const double roofAngleDegrees;

    static constexpr double pi = 3.14159265358979323846;
    static constexpr int numBaysBottom = 5;
    static constexpr int numBaysDiagonal = 3;
};

#endif //C_EXERCISE_TRUSSCALCULATOR_H
