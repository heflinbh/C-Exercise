#include <iostream>
#include "GeometryCalculator.h"

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

    GeometryCalculator calculator(trussSpanMm, roofAngleDegrees);

    std::cout << "\nCalculated geometry:\n";
    std::cout << "Bottom length: " << calculator.getLengthOfBottomMm() << " mm\n";
    std::cout << "Diagonal length (each side): " << calculator.getLengthOfDiagonalMm() << " mm\n";
    std::cout << "Bottom bay length (5 bays): " << calculator.getBayLengthOfBottomMm() << " mm\n";
    std::cout << "Diagonal bay length (3 bays/side): " << calculator.getBayLengthOfDiagonalMm() << " mm\n";

    return 0;
}
