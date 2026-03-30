#include <iostream>
#include <map>
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

    std::vector<int> stock{600, 900, 1200, 1500, 1800, 2100, 2400, 2700, 3000};

    CuttingPlanner planner(stock);
    const CutPlan plan = planner.planGreedy(constraints);

    std::cout << "\n=== Truss Cut Plan (Greedy) ===\n";
    std::cout << "Total truss waste: " << plan.totalWasteMm << " mm\n";

    std::cout << "\n--- Bottom Member ---\n";
    std::cout << "Total bottom waste: " << plan.bottom.totalWasteMm << " mm\n";
    std::map<int, int> bottomStockUsage;
    for (size_t i = 0; i < plan.bottom.pieces.size(); ++i) {
        const CutPiece& piece = plan.bottom.pieces[i];
        bottomStockUsage[piece.originalStockLengthMm] += 1;
        std::cout << "Piece " << (i + 1)
                  << ": length=" << piece.lengthOfPieceMm
                  << " mm, stock=" << piece.originalStockLengthMm
                  << " mm, waste=" << piece.wasteMm << " mm\n";
    }
    std::cout << "Bottom stock usage:\n";
    for (const auto& entry : bottomStockUsage) {
        std::cout << "  " << entry.first << " mm x " << entry.second << "\n";
    }

    std::cout << "\n--- Left Diagonal Member ---\n";
    std::cout << "Total left diagonal waste: " << plan.diagonalLeft.totalWasteMm << " mm\n";
    std::map<int, int> leftDiagStockUsage;
    for (size_t i = 0; i < plan.diagonalLeft.pieces.size(); ++i) {
        const CutPiece& piece = plan.diagonalLeft.pieces[i];
        leftDiagStockUsage[piece.originalStockLengthMm] += 1;
        std::cout << "Piece " << (i + 1)
                  << ": length=" << piece.lengthOfPieceMm
                  << " mm, stock=" << piece.originalStockLengthMm
                  << " mm, waste=" << piece.wasteMm << " mm\n";
    }
    std::cout << "Left diagonal stock usage:\n";
    for (const auto& entry : leftDiagStockUsage) {
        std::cout << "  " << entry.first << " mm x " << entry.second << "\n";
    }

    std::cout << "\n--- Right Diagonal Member ---\n";
    std::cout << "Total right diagonal waste: " << plan.diagonalRight.totalWasteMm << " mm\n";
    std::map<int, int> rightDiagStockUsage;
    for (size_t i = 0; i < plan.diagonalRight.pieces.size(); ++i) {
        const CutPiece& piece = plan.diagonalRight.pieces[i];
        rightDiagStockUsage[piece.originalStockLengthMm] += 1;
        std::cout << "Piece " << (i + 1)
                  << ": length=" << piece.lengthOfPieceMm
                  << " mm, stock=" << piece.originalStockLengthMm
                  << " mm, waste=" << piece.wasteMm << " mm\n";
    }
    std::cout << "Right diagonal stock usage:\n";
    for (const auto& entry : rightDiagStockUsage) {
        std::cout << "  " << entry.first << " mm x " << entry.second << "\n";
    }

    std::cout << "\n=== Combined Stock Usage (Full Truss) ===\n";
    std::map<int, int> combinedStockUsage;
    for (const auto& entry : bottomStockUsage) {
        combinedStockUsage[entry.first] += entry.second;
    }
    for (const auto& entry : leftDiagStockUsage) {
        combinedStockUsage[entry.first] += entry.second;
    }
    for (const auto& entry : rightDiagStockUsage) {
        combinedStockUsage[entry.first] += entry.second;
    }
    for (const auto& entry : combinedStockUsage) {
        std::cout << entry.first << " mm x " << entry.second << "\n";
    }

    return 0;
}
