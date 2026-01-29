//
// Created by QuAn on 26-1-29.
//
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include "Math/translation_matrix.h"

using namespace CS::Math;

void printMatrix(const Matrix& mat) {
    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < 4; ++i) {
        std::cout << "[ ";
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << mat.M[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

void printVector(const Vector4d& vec, const std::string& name) {
    std::cout << std::setw(25) << name << ": ["
              << std::setw(8) << vec.x << ", "
              << std::setw(8) << vec.y << ", "
              << std::setw(8) << vec.z << ", "
              << std::setw(8) << vec.w << "]" << std::endl;
}

bool compareVectors(const Vector4d& v1, const Vector4d& v2) {
    return fabs(v1.x - v2.x) < 1e-4 &&
           fabs(v1.y - v2.y) < 1e-4 &&
           fabs(v1.z - v2.z) < 1e-4 &&
           fabs(v1.w - v2.w) < 1e-4;
}

bool compareMatrices(const Matrix& m1, const Matrix& m2) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (fabs(m1.M[i][j] - m2.M[i][j]) > 1e-4) {
                return false;
            }
        }
    }
    return true;
}

bool testCase1() {
    std::cout << "Test Case 1: Zero translation (identity matrix)" << std::endl;

    Vector3d translation(0.0, 0.0, 0.0);
    TranslationMatrix transMat(translation);

    Matrix identity;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            identity.M[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    bool passed = compareMatrices(transMat, identity);

    if (!passed) {
        std::cout << "Expected identity matrix:" << std::endl;
        printMatrix(identity);
        std::cout << "Got:" << std::endl;
        printMatrix(transMat);
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase2() {
    std::cout << "Test Case 2: Translation along X axis" << std::endl;

    Vector3d translation(5.0, 0.0, 0.0);
    TranslationMatrix transMat(translation);

    Vector4d point;
    point.x = 1.0;
    point.y = 2.0;
    point.z = 3.0;
    point.w = 1.0;

    Vector4d result = transMat * point;

    Vector4d expected;
    expected.x = 6.0;  // 1 + 5
    expected.y = 2.0;  // 2 + 0
    expected.z = 3.0;  // 3 + 0
    expected.w = 1.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(point, "Original point");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase3() {
    std::cout << "Test Case 3: Translation along Y axis" << std::endl;

    Vector3d translation(0.0, 3.0, 0.0);
    TranslationMatrix transMat(translation);

    Vector4d point;
    point.x = 2.0;
    point.y = 1.0;
    point.z = 4.0;
    point.w = 1.0;

    Vector4d result = transMat * point;

    Vector4d expected;
    expected.x = 2.0;  // 2 + 0
    expected.y = 4.0;  // 1 + 3
    expected.z = 4.0;  // 4 + 0
    expected.w = 1.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(point, "Original point");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase4() {
    std::cout << "Test Case 4: Translation along Z axis" << std::endl;

    Vector3d translation(0.0, 0.0, 2.5);
    TranslationMatrix transMat(translation);

    Vector4d point;
    point.x = 1.0;
    point.y = 1.0;
    point.z = 1.0;
    point.w = 1.0;

    Vector4d result = transMat * point;

    Vector4d expected;
    expected.x = 1.0;    // 1 + 0
    expected.y = 1.0;    // 1 + 0
    expected.z = 3.5;    // 1 + 2.5
    expected.w = 1.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(point, "Original point");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase5() {
    std::cout << "Test Case 5: 3D translation (all axes)" << std::endl;

    Vector3d translation(2.0, 3.0, 4.0);
    TranslationMatrix transMat(translation);

    Vector4d point;
    point.x = 1.0;
    point.y = 1.0;
    point.z = 1.0;
    point.w = 1.0;

    Vector4d result = transMat * point;

    Vector4d expected;
    expected.x = 3.0;  // 1 + 2
    expected.y = 4.0;  // 1 + 3
    expected.z = 5.0;  // 1 + 4
    expected.w = 1.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(point, "Original point");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase6() {
    std::cout << "Test Case 6: Translation with direction vector (w=0)" << std::endl;

    Vector3d translation(2.0, 3.0, 4.0);
    TranslationMatrix transMat(translation);

    Vector4d direction;
    direction.x = 1.0;
    direction.y = 0.0;
    direction.z = 0.0;
    direction.w = 0.0;

    Vector4d result = transMat * direction;

    Vector4d expected;
    expected.x = 1.0;  // Direction should NOT be translated
    expected.y = 0.0;
    expected.z = 0.0;
    expected.w = 0.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(direction, "Original direction");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase7() {
    std::cout << "Test Case 7: Make function test" << std::endl;

    Vector3d translation1(1.0, 2.0, 3.0);
    TranslationMatrix transMat1(translation1);

    Vector3d translation2(4.0, 5.0, 6.0);
    transMat1.Make(translation2);

    // Create a new matrix with translation2 to compare
    TranslationMatrix transMat2(translation2);

    bool passed = compareMatrices(transMat1, transMat2);

    if (!passed) {
        std::cout << "After Make function:" << std::endl;
        printMatrix(transMat1);
        std::cout << "Expected:" << std::endl;
        printMatrix(transMat2);
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase8() {
    std::cout << "Test Case 8: Negative translation" << std::endl;

    Vector3d translation(-2.0, -3.0, -4.0);
    TranslationMatrix transMat(translation);

    Vector4d point;
    point.x = 5.0;
    point.y = 5.0;
    point.z = 5.0;
    point.w = 1.0;

    Vector4d result = transMat * point;

    Vector4d expected;
    expected.x = 3.0;  // 5 - 2
    expected.y = 2.0;  // 5 - 3
    expected.z = 1.0;  // 5 - 4
    expected.w = 1.0;

    bool passed = compareVectors(result, expected);

    if (!passed) {
        printVector(point, "Original point");
        printVector(expected, "Expected result");
        printVector(result, "Actual result");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase9() {
    std::cout << "Test Case 9: Matrix structure verification" << std::endl;

    Vector3d translation(2.0, 3.0, 4.0);
    TranslationMatrix transMat(translation);

    bool passed = true;

    // Check 3x3 identity part
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) {
                if (fabs(transMat.M[i][j] - 1.0) > 1e-4) {
                    passed = false;
                    std::cout << "Error at M[" << i << "][" << j << "]: expected 1.0, got " << transMat.M[i][j] << std::endl;
                }
            } else {
                if (fabs(transMat.M[i][j]) > 1e-4) {
                    passed = false;
                    std::cout << "Error at M[" << i << "][" << j << "]: expected 0.0, got " << transMat.M[i][j] << std::endl;
                }
            }
        }
    }

    // Check translation components
    if (fabs(transMat.M[0][3] - 2.0) > 1e-4) {
        passed = false;
        std::cout << "Error at M[0][3]: expected 2.0, got " << transMat.M[0][3] << std::endl;
    }
    if (fabs(transMat.M[1][3] - 3.0) > 1e-4) {
        passed = false;
        std::cout << "Error at M[1][3]: expected 3.0, got " << transMat.M[1][3] << std::endl;
    }
    if (fabs(transMat.M[2][3] - 4.0) > 1e-4) {
        passed = false;
        std::cout << "Error at M[2][3]: expected 4.0, got " << transMat.M[2][3] << std::endl;
    }

    // Check last row
    for (int j = 0; j < 4; ++j) {
        if (j == 3) {
            if (fabs(transMat.M[3][j] - 1.0) > 1e-4) {
                passed = false;
                std::cout << "Error at M[3][" << j << "]: expected 1.0, got " << transMat.M[3][j] << std::endl;
            }
        } else {
            if (fabs(transMat.M[3][j]) > 1e-4) {
                passed = false;
                std::cout << "Error at M[3][" << j << "]: expected 0.0, got " << transMat.M[3][j] << std::endl;
            }
        }
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase10() {
    std::cout << "Test Case 10: Multiple point translations" << std::endl;

    Vector3d translation(1.0, 2.0, 3.0);
    TranslationMatrix transMat(translation);

    Vector4d points[5];
    Vector4d expected[5];

    // Point 1
    points[0].x = 0.0; points[0].y = 0.0; points[0].z = 0.0; points[0].w = 1.0;
    expected[0].x = 1.0; expected[0].y = 2.0; expected[0].z = 3.0; expected[0].w = 1.0;

    // Point 2
    points[1].x = 1.0; points[1].y = 0.0; points[1].z = 0.0; points[1].w = 1.0;
    expected[1].x = 2.0; expected[1].y = 2.0; expected[1].z = 3.0; expected[1].w = 1.0;

    // Point 3
    points[2].x = 0.0; points[2].y = 1.0; points[2].z = 0.0; points[2].w = 1.0;
    expected[2].x = 1.0; expected[2].y = 3.0; expected[2].z = 3.0; expected[2].w = 1.0;

    // Point 4
    points[3].x = 0.0; points[3].y = 0.0; points[3].z = 1.0; points[3].w = 1.0;
    expected[3].x = 1.0; expected[3].y = 2.0; expected[3].z = 4.0; expected[3].w = 1.0;

    // Point 5
    points[4].x = -1.0; points[4].y = -2.0; points[4].z = -3.0; points[4].w = 1.0;
    expected[4].x = 0.0; expected[4].y = 0.0; expected[4].z = 0.0; expected[4].w = 1.0;

    bool allPassed = true;

    for (int i = 0; i < 5; ++i) {
        Vector4d result = transMat * points[i];
        if (!compareVectors(result, expected[i])) {
            allPassed = false;
            std::cout << "Point " << i + 1 << " failed:" << std::endl;
            printVector(points[i], "Original");
            printVector(expected[i], "Expected");
            printVector(result, "Actual");
        }
    }

    std::cout << "Result: " << (allPassed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return allPassed;
}

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "TRANSLATION MATRIX TEST SUITE" << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    int passedCount = 0;
    int totalCount = 10;

    if (testCase1()) passedCount++;
    if (testCase2()) passedCount++;
    if (testCase3()) passedCount++;
    if (testCase4()) passedCount++;
    if (testCase5()) passedCount++;
    if (testCase6()) passedCount++;
    if (testCase7()) passedCount++;
    if (testCase8()) passedCount++;
    if (testCase9()) passedCount++;
    if (testCase10()) passedCount++;

    std::cout << "==================================================" << std::endl;
    std::cout << "TEST SUMMARY" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Tests passed: " << passedCount << " out of " << totalCount << std::endl;
    std::cout << "Success rate: " << (passedCount * 100 / totalCount) << "%" << std::endl;

    if (passedCount == totalCount) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
    }

    return 0;
}
