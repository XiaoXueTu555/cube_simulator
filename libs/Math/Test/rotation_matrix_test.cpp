// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include <string>
// #include "Math/rotation_matrix.h"
//
// using namespace CS::Math;
//
// void printMatrix(const Matrix& mat) {
//     std::cout << std::fixed << std::setprecision(4);
//     for (int i = 0; i < 4; ++i) {
//         std::cout << "[ ";
//         for (int j = 0; j < 4; ++j) {
//             std::cout << std::setw(8) << mat.M[i][j] << " ";
//         }
//         std::cout << "]" << std::endl;
//     }
// }
//
// void printVector(const Vector4d& vec, const std::string& name) {
//     std::cout << std::setw(20) << name << ": ["
//               << std::setw(8) << vec.x << ", "
//               << std::setw(8) << vec.y << ", "
//               << std::setw(8) << vec.z << ", "
//               << std::setw(8) << vec.w << "]" << std::endl;
// }
//
// bool compareVectors(const Vector4d& v1, const Vector4d& v2) {
//     return fabs(v1.x - v2.x) < 1e-4 &&
//            fabs(v1.y - v2.y) < 1e-4 &&
//            fabs(v1.z - v2.z) < 1e-4 &&
//            fabs(v1.w - v2.w) < 1e-4;
// }
//
// bool checkMatrixEquality(const Matrix& m1, const Matrix& m2) {
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (fabs(m1.M[i][j] - m2.M[i][j]) > 1e-4) {
//                 return false;
//             }
//         }
//     }
//     return true;
// }
//
// bool testCase1() {
//     std::cout << "Test Case 1: Identity rotation (0,0,0)" << std::endl;
//
//     Vector3d angles(0.0, 0.0, 0.0);
//     RotationMatrix rotMat(angles);
//
//     Vector4d testVec;
//     testVec.x = 1.0;
//     testVec.y = 2.0;
//     testVec.z = 3.0;
//     testVec.w = 1.0;
//
//     Vector4d result = rotMat * testVec;
//
//     Vector4d expected;
//     expected.x = 1.0;
//     expected.y = 2.0;
//     expected.z = 3.0;
//     expected.w = 1.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase2() {
//     std::cout << "Test Case 2: 90-degree rotation around X axis" << std::endl;
//
//     double angleX = 3.14159265358979323846 / 2.0;
//     Vector3d angles(angleX, 0.0, 0.0);
//     RotationMatrix rotMat(angles);
//
//     Vector4d testVec;
//     testVec.x = 0.0;
//     testVec.y = 1.0;
//     testVec.z = 0.0;
//     testVec.w = 1.0;
//
//     Vector4d result = rotMat * testVec;
//
//     Vector4d expected;
//     expected.x = 0.0;
//     expected.y = 0.0;
//     expected.z = 1.0;
//     expected.w = 1.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase3() {
//     std::cout << "Test Case 3: 90-degree rotation around Y axis" << std::endl;
//
//     double angleY = 3.14159265358979323846 / 2.0;
//     Vector3d angles(0.0, angleY, 0.0);
//     RotationMatrix rotMat(angles);
//
//     Vector4d testVec;
//     testVec.x = 1.0;
//     testVec.y = 0.0;
//     testVec.z = 0.0;
//     testVec.w = 1.0;
//
//     Vector4d result = rotMat * testVec;
//
//     Vector4d expected;
//     expected.x = 0.0;
//     expected.y = 0.0;
//     expected.z = -1.0;
//     expected.w = 1.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase4() {
//     std::cout << "Test Case 4: 90-degree rotation around Z axis" << std::endl;
//
//     double angleZ = 3.14159265358979323846 / 2.0;
//     Vector3d angles(0.0, 0.0, angleZ);
//     RotationMatrix rotMat(angles);
//
//     Vector4d testVec;
//     testVec.x = 1.0;
//     testVec.y = 0.0;
//     testVec.z = 0.0;
//     testVec.w = 1.0;
//
//     Vector4d result = rotMat * testVec;
//
//     Vector4d expected;
//     expected.x = 0.0;
//     expected.y = 1.0;
//     expected.z = 0.0;
//     expected.w = 1.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase5() {
//     std::cout << "Test Case 5: Direction vector rotation (w=0)" << std::endl;
//
//     double angleX = 3.14159265358979323846 / 2.0;
//     Vector3d angles(angleX, 0.0, 0.0);
//     RotationMatrix rotMat(angles);
//
//     Vector4d direction;
//     direction.x = 0.0;
//     direction.y = 1.0;
//     direction.z = 0.0;
//     direction.w = 0.0;
//
//     Vector4d result = rotMat * direction;
//
//     Vector4d expected;
//     expected.x = 0.0;
//     expected.y = 0.0;
//     expected.z = 1.0;
//     expected.w = 0.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase6() {
//     std::cout << "Test Case 6: Composite rotation (45,30,60 degrees)" << std::endl;
//
//     double degToRad = 3.14159265358979323846 / 180.0;
//     Vector3d angles(45.0 * degToRad, 30.0 * degToRad, 60.0 * degToRad);
//     RotationMatrix rotMat(angles);
//
//     Vector4d testVec;
//     testVec.x = 1.0;
//     testVec.y = 0.0;
//     testVec.z = 0.0;
//     testVec.w = 1.0;
//
//     Vector4d result = rotMat * testVec;
//
//     Vector4d expected;
//     expected.x = 0.4330;
//     expected.y = 0.7500;
//     expected.z = -0.5000;
//     expected.w = 1.0;
//
//     bool passed = compareVectors(result, expected);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase7() {
//     std::cout << "Test Case 7: Rotation matrix orthogonality" << std::endl;
//
//     double degToRad = 3.14159265358979323846 / 180.0;
//     Vector3d angles(20.0 * degToRad, 40.0 * degToRad, 70.0 * degToRad);
//     RotationMatrix R(angles);
//
//     double error = 0.0;
//     for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 3; ++j) {
//             double dot = 0.0;
//             for (int k = 0; k < 3; ++k) {
//                 dot += R.M[i][k] * R.M[j][k];
//             }
//             double expected = (i == j) ? 1.0 : 0.0;
//             error += fabs(dot - expected);
//         }
//     }
//
//     bool passed = (error < 1e-4);
//     std::cout << "Orthogonality error: " << std::setprecision(4) << error << std::endl;
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase8() {
//     std::cout << "Test Case 8: Determinant of rotation matrix" << std::endl;
//
//     double degToRad = 3.14159265358979323846 / 180.0;
//     Vector3d angles(15.0 * degToRad, 25.0 * degToRad, 35.0 * degToRad);
//     RotationMatrix R(angles);
//
//     double det =
//         R.M[0][0] * (R.M[1][1] * R.M[2][2] - R.M[1][2] * R.M[2][1]) -
//         R.M[0][1] * (R.M[1][0] * R.M[2][2] - R.M[1][2] * R.M[2][0]) +
//         R.M[0][2] * (R.M[1][0] * R.M[2][1] - R.M[1][1] * R.M[2][0]);
//
//     bool passed = fabs(det - 1.0) < 1e-4;
//     std::cout << "Determinant: " << std::setprecision(4) << det << std::endl;
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase9() {
//     std::cout << "Test Case 9: Inverse rotation property" << std::endl;
//
//     double degToRad = 3.14159265358979323846 / 180.0;
//     Vector3d angles(30.0 * degToRad, 45.0 * degToRad, 60.0 * degToRad);
//     RotationMatrix R(angles);
//
//     RotationMatrix Rinv(angles);
//     for (int i = 0; i < 3; ++i) {
//         for (int j = i+1; j < 3; ++j) {
//             std::swap(Rinv.M[i][j], Rinv.M[j][i]);
//         }
//     }
//
//     Matrix identity;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             identity.M[i][j] = (i == j) ? 1.0 : 0.0;
//         }
//     }
//
//     Matrix product;
//     for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 3; ++j) {
//             product.M[i][j] = 0.0;
//             for (int k = 0; k < 3; ++k) {
//                 product.M[i][j] += R.M[i][k] * Rinv.M[k][j];
//             }
//         }
//     }
//     product.M[3][3] = 1.0;
//
//     bool passed = checkMatrixEquality(product, identity);
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase10() {
//     std::cout << "Test Case 10: Vector length preservation" << std::endl;
//
//     double degToRad = 3.14159265358979323846 / 180.0;
//     Vector3d angles(10.0 * degToRad, 20.0 * degToRad, 30.0 * degToRad);
//     RotationMatrix R(angles);
//
//     Vector4d testVec;
//     testVec.x = 3.0;
//     testVec.y = 4.0;
//     testVec.z = 5.0;
//     testVec.w = 1.0;
//
//     Vector4d rotated = R * testVec;
//
//     double originalLength = sqrt(testVec.x * testVec.x + testVec.y * testVec.y + testVec.z * testVec.z);
//     double rotatedLength = sqrt(rotated.x * rotated.x + rotated.y * rotated.y + rotated.z * rotated.z);
//
//     bool passed = fabs(originalLength - rotatedLength) < 1e-4;
//     std::cout << "Original length: " << std::setprecision(4) << originalLength << std::endl;
//     std::cout << "Rotated length: " << std::setprecision(4) << rotatedLength << std::endl;
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// int main() {
//     std::cout << "==================================================" << std::endl;
//     std::cout << "ROTATION MATRIX TEST SUITE" << std::endl;
//     std::cout << "==================================================" << std::endl << std::endl;
//
//     int passedCount = 0;
//     int totalCount = 10;
//
//     if (testCase1()) passedCount++;
//     if (testCase2()) passedCount++;
//     if (testCase3()) passedCount++;
//     if (testCase4()) passedCount++;
//     if (testCase5()) passedCount++;
//     if (testCase6()) passedCount++;
//     if (testCase7()) passedCount++;
//     if (testCase8()) passedCount++;
//     if (testCase9()) passedCount++;
//     if (testCase10()) passedCount++;
//
//     std::cout << "==================================================" << std::endl;
//     std::cout << "TEST SUMMARY" << std::endl;
//     std::cout << "==================================================" << std::endl;
//     std::cout << "Tests passed: " << passedCount << " out of " << totalCount << std::endl;
//     std::cout << "Success rate: " << (passedCount * 100 / totalCount) << "%" << std::endl;
//
//     if (passedCount == totalCount) {
//         std::cout << "ALL TESTS PASSED" << std::endl;
//     } else {
//         std::cout << "SOME TESTS FAILED" << std::endl;
//     }
//
//     return 0;
// }
