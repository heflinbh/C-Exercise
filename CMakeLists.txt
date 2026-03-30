cmake_minimum_required(VERSION 3.14)
project(C_Exercise)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

find_package(ZLIB REQUIRED)

add_executable(C_Exercise_P1
        "Problem 1 - XML Exercise/main.cpp"
        "Problem 1 - XML Exercise/BenXMLOutput.cpp"
        "Problem 1 - XML Exercise/BenXMLOutput.h"
        "Problem 1 - XML Exercise/Serializer.cpp"
        "Problem 1 - XML Exercise/Serializer.h"
)
target_link_libraries(C_Exercise_P1 PRIVATE ZLIB::ZLIB)

add_executable(C_Exercise_P2
        "Problem 2 - Roof Truss/main.cpp"
        "Problem 2 - Roof Truss/BenCustomTrussConstraintProvider.cpp"
        "Problem 2 - Roof Truss/BenCustomTrussConstraintProvider.h"
        "Problem 2 - Roof Truss/ITrussConstraintProvider.h"
        "Problem 2 - Roof Truss/TrussModels.h"
        "Problem 2 - Roof Truss/GreedySingleMemberSolver.cpp"
        "Problem 2 - Roof Truss/GreedySingleMemberSolver.h"
        "Problem 2 - Roof Truss/ISingleMemberSolver.h"
        "Problem 2 - Roof Truss/CuttingPlanner.cpp"
        "Problem 2 - Roof Truss/CuttingPlanner.h"
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(SerializerTests
        "Problem 1 - XML Exercise/SerializerTests.cpp"
        "Problem 1 - XML Exercise/Serializer.cpp"
        "Problem 1 - XML Exercise/Serializer.h")
target_link_libraries(SerializerTests PRIVATE GTest::gtest_main)

add_executable(BenXMLOutputTests
        "Problem 1 - XML Exercise/BenXMLOutputTests.cpp"
        "Problem 1 - XML Exercise/BenXMLOutput.cpp"
        "Problem 1 - XML Exercise/BenXMLOutput.h"
        "Problem 1 - XML Exercise/Serializer.cpp"
        "Problem 1 - XML Exercise/Serializer.h"
)
target_link_libraries(BenXMLOutputTests PRIVATE GTest::gtest_main ZLIB::ZLIB)

include(GoogleTest)
gtest_discover_tests(SerializerTests)
gtest_discover_tests(BenXMLOutputTests)
