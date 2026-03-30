#include <iostream>
#include "BenCustomTrussConstraintProvider.h"
#include "CuttingPlanner.h"

namespace {
    constexpr double pi = 3.14159265358979323846;

    double degreesToRadians(double degrees) {
        return degrees * pi / 180.0;
    }
}

int main() {
    int trussSpanMm = 0;
    double roofAngleDegrees = 0.0;

    std::cout << "Enter bottom truss span (mm): ";
    std::cin >> trussSpanMm;

    if (!std::cin || trussSpanMm <= 0) {
        std::cerr << "Invalid truss span. Please enter a valid number.\n";
        return 1;
    }

    std::cout << "Enter roof angle value (deg): ";
    std::cin >> roofAngleDegrees;

    if (!std::cin || roofAngleDegrees <= 0.0 || roofAngleDegrees >= 90.0) {
        std::cerr << "Invalid roof angle. Enter a value between 0 and 90 degrees.\n";
        return 1;
    }

    const double roofAngleRad = degreesToRadians(roofAngleDegrees);

    TrussInput input{trussSpanMm, roofAngleRad};

    BenCustomTrussConstraintProvider provider;
    TrussConstraints constraints = provider.build(input);

    std::vector stock{600, 900, 1200, 1500, 1800, 2100, 2400, 2700, 3000};

    CuttingPlanner planner(stock);
    const CutPlan plan = planner.planGreedy(constraints);

    return 0;
}
