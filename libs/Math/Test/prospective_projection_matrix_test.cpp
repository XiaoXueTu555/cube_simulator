// //
// // Created by QuAn on 26-1-29.
// //
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include <string>
// #include "Math/perspective_projection_matrix.h"
// #include "Math/vector3d.h"
// #include "Math/vector4d.h"
//
// using namespace CS::Math;
//
// void printMatrix(const Matrix& mat) {
//     std::cout << std::fixed << std::setprecision(4);
//     for (int i = 0; i < 4; ++i) {
//         std::cout << "[ ";
//         for (int j = 0; j < 4; ++j) {
//             std::cout << std::setw(10) << mat.M[i][j] << " ";
//         }
//         std::cout << "]" << std::endl;
//     }
// }
//
// void printVector(const Vector4d& vec, const std::string& name) {
//     std::cout << std::setw(25) << name << ": ["
//               << std::setw(10) << vec.x << ", "
//               << std::setw(10) << vec.y << ", "
//               << std::setw(10) << vec.z << ", "
//               << std::setw(10) << vec.w << "]" << std::endl;
// }
//
// Vector4d perspectiveDivide(const Vector4d& vec) {
//     Vector4d result;
//     if (fabs(vec.w) > 1e-10) {
//         result.x = vec.x / vec.w;
//         result.y = vec.y / vec.w;
//         result.z = vec.z / vec.w;
//         result.w = 1.0f;
//     } else {
//         result = vec;
//     }
//     return result;
// }
//
// bool compareVectors(const Vector4d& v1, const Vector4d& v2) {
//     return fabs(v1.x - v2.x) < 1e-4 &&
//            fabs(v1.y - v2.y) < 1e-4 &&
//            fabs(v1.z - v2.z) < 1e-4 &&
//            fabs(v1.w - v2.w) < 1e-4;
// }
//
// bool testCase1() {
//     std::cout << "Test Case 1: Standard perspective projection matrix structure" << std::endl;
//
//     float fov = 90.0f;  // 90 degrees
//     float nearPlane = 0.1f;
//     float farPlane = 100.0f;
//     float aspectRatio = 16.0f / 9.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Calculate expected values
//     float fovRad = fov * 3.14159265358979323846f / 180.0f;
//     float f = 1.0f / tan(fovRad * 0.5f);
//     float a = (farPlane + nearPlane) / (nearPlane - farPlane);
//     float b = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
//
//     bool passed = true;
//
//     // Check specific matrix elements
//     if (fabs(projMat.M[0][0] - (f / aspectRatio)) > 1e-4) {
//         passed = false;
//         std::cout << "Error at M[0][0]: expected " << (f / aspectRatio)
//                   << ", got " << projMat.M[0][0] << std::endl;
//     }
//
//     if (fabs(projMat.M[1][1] - f) > 1e-4) {
//         passed = false;
//         std::cout << "Error at M[1][1]: expected " << f
//                   << ", got " << projMat.M[1][1] << std::endl;
//     }
//
//     if (fabs(projMat.M[2][2] - a) > 1e-4) {
//         passed = false;
//         std::cout << "Error at M[2][2]: expected " << a
//                   << ", got " << projMat.M[2][2] << std::endl;
//     }
//
//     if (fabs(projMat.M[2][3] - b) > 1e-4) {
//         passed = false;
//         std::cout << "Error at M[2][3]: expected " << b
//                   << ", got " << projMat.M[2][3] << std::endl;
//     }
//
//     if (fabs(projMat.M[3][2] - (-1.0f)) > 1e-4) {
//         passed = false;
//         std::cout << "Error at M[3][2]: expected -1.0, got " << projMat.M[3][2] << std::endl;
//     }
//
//     // Check zeros where they should be zero
//     if (fabs(projMat.M[0][1]) > 1e-4 || fabs(projMat.M[0][2]) > 1e-4 || fabs(projMat.M[0][3]) > 1e-4 ||
//         fabs(projMat.M[1][0]) > 1e-4 || fabs(projMat.M[1][2]) > 1e-4 || fabs(projMat.M[1][3]) > 1e-4 ||
//         fabs(projMat.M[2][0]) > 1e-4 || fabs(projMat.M[2][1]) > 1e-4 ||
//         fabs(projMat.M[3][0]) > 1e-4 || fabs(projMat.M[3][1]) > 1e-4 || fabs(projMat.M[3][3]) > 1e-4) {
//         passed = false;
//         std::cout << "Matrix has non-zero elements where zero is expected" << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase2() {
//     std::cout << "Test Case 2: Point on near plane projects to -1 in NDC Z" << std::endl;
//
//     float fov = 60.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//     float aspectRatio = 1.0f;  // Square aspect ratio for simplicity
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Point on near plane (z = -near in view space, but our matrix expects z negative)
//     // For perspective projection, points are in view space where camera looks along -Z
//     Vector4d point;
//     point.x = 0.0f;
//     point.y = 0.0f;
//     point.z = -nearPlane;  // On near plane in view space
//     point.w = 1.0f;
//
//     Vector4d projected = projMat * point;
//     Vector4d ndc = perspectiveDivide(projected);
//
//     bool passed = fabs(ndc.z - (-1.0f)) < 1e-4;
//
//     if (!passed) {
//         printVector(point, "View space point");
//         printVector(projected, "Projected (clip space)");
//         printVector(ndc, "NDC after perspective divide");
//         std::cout << "Expected NDC.z = -1.0" << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase3() {
//     std::cout << "Test Case 3: Point on far plane projects to 1 in NDC Z" << std::endl;
//
//     float fov = 60.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//     float aspectRatio = 1.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Point on far plane
//     Vector4d point;
//     point.x = 0.0f;
//     point.y = 0.0f;
//     point.z = -farPlane;  // On far plane in view space
//     point.w = 1.0f;
//
//     Vector4d projected = projMat * point;
//     Vector4d ndc = perspectiveDivide(projected);
//
//     bool passed = fabs(ndc.z - 1.0f) < 1e-4;
//
//     if (!passed) {
//         printVector(point, "View space point");
//         printVector(projected, "Projected (clip space)");
//         printVector(ndc, "NDC after perspective divide");
//         std::cout << "Expected NDC.z = 1.0" << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase4() {
//     std::cout << "Test Case 4: Point at center of view frustum" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//     float aspectRatio = 1.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Point halfway between near and far planes
//     Vector4d point;
//     point.x = 0.0f;
//     point.y = 0.0f;
//     point.z = -(nearPlane + farPlane) / 2.0f;
//     point.w = 1.0f;
//
//     Vector4d projected = projMat * point;
//     Vector4d ndc = perspectiveDivide(projected);
//
//     // X and Y should be 0 at center
//     bool passed = fabs(ndc.x) < 1e-4 && fabs(ndc.y) < 1e-4;
//
//     if (!passed) {
//         printVector(point, "View space point");
//         printVector(projected, "Projected (clip space)");
//         printVector(ndc, "NDC after perspective divide");
//         std::cout << "Expected NDC.x = 0, NDC.y = 0" << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase5() {
//     std::cout << "Test Case 5: Point at right frustum boundary" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//     float aspectRatio = 16.0f / 9.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Calculate right boundary at near plane
//     float fovRad = fov * 3.14159265358979323846f / 180.0f;
//     float tanHalfFov = tan(fovRad * 0.5f);
//     float rightBoundary = nearPlane * tanHalfFov * aspectRatio;
//
//     // Point at right boundary on near plane
//     Vector4d point;
//     point.x = rightBoundary;
//     point.y = 0.0f;
//     point.z = -nearPlane;
//     point.w = 1.0f;
//
//     Vector4d projected = projMat * point;
//     Vector4d ndc = perspectiveDivide(projected);
//
//     // Should project to x = 1 in NDC
//     bool passed = fabs(ndc.x - 1.0f) < 1e-4 && fabs(ndc.y) < 1e-4;
//
//     if (!passed) {
//         printVector(point, "View space point");
//         printVector(projected, "Projected (clip space)");
//         printVector(ndc, "NDC after perspective divide");
//         std::cout << "Expected NDC.x = 1.0, NDC.y = 0" << std::endl;
//         std::cout << "Right boundary in view space: " << rightBoundary << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase6() {
//     std::cout << "Test Case 6: Point at top frustum boundary" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//     float aspectRatio = 16.0f / 9.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Calculate top boundary at near plane
//     float fovRad = fov * 3.14159265358979323846f / 180.0f;
//     float tanHalfFov = tan(fovRad * 0.5f);
//     float topBoundary = nearPlane * tanHalfFov;
//
//     // Point at top boundary on near plane
//     Vector4d point;
//     point.x = 0.0f;
//     point.y = topBoundary;
//     point.z = -nearPlane;
//     point.w = 1.0f;
//
//     Vector4d projected = projMat * point;
//     Vector4d ndc = perspectiveDivide(projected);
//
//     // Should project to y = 1 in NDC
//     bool passed = fabs(ndc.y - 1.0f) < 1e-4 && fabs(ndc.x) < 1e-4;
//
//     if (!passed) {
//         printVector(point, "View space point");
//         printVector(projected, "Projected (clip space)");
//         printVector(ndc, "NDC after perspective divide");
//         std::cout << "Expected NDC.y = 1.0, NDC.x = 0" << std::endl;
//         std::cout << "Top boundary in view space: " << topBoundary << std::endl;
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase7() {
//     std::cout << "Test Case 7: Make function test" << std::endl;
//
//     float fov1 = 60.0f;
//     float near1 = 0.5f;
//     float far1 = 50.0f;
//     float aspect1 = 4.0f / 3.0f;
//
//     PerspectiveProjectionMatrix projMat(fov1, near1, far1, aspect1);
//
//     float fov2 = 120.0f;
//     float near2 = 0.1f;
//     float far2 = 200.0f;
//     float aspect2 = 2.0f;
//
//     projMat.Make(fov2, near2, far2, aspect2);
//
//     // Create a second matrix with same parameters
//     PerspectiveProjectionMatrix projMat2(fov2, near2, far2, aspect2);
//
//     bool passed = true;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (fabs(projMat.M[i][j] - projMat2.M[i][j]) > 1e-4) {
//                 passed = false;
//                 std::cout << "Matrix mismatch at [" << i << "][" << j << "]: "
//                           << projMat.M[i][j] << " vs " << projMat2.M[i][j] << std::endl;
//             }
//         }
//     }
//
//     std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return passed;
// }
//
// bool testCase8() {
//     std::cout << "Test Case 8: Different aspect ratios" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 100.0f;
//
//     // Test with different aspect ratios
//     float aspectRatios[] = {1.0f, 4.0f/3.0f, 16.0f/9.0f, 2.35f};
//     bool allPassed = true;
//
//     for (int i = 0; i < 4; ++i) {
//         float aspectRatio = aspectRatios[i];
//         PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//         // Check that M[0][0] = f / aspectRatio
//         float fovRad = fov * 3.14159265358979323846f / 180.0f;
//         float f = 1.0f / tan(fovRad * 0.5f);
//         float expected = f / aspectRatio;
//
//         if (fabs(projMat.M[0][0] - expected) > 1e-4) {
//             allPassed = false;
//             std::cout << "Aspect ratio " << aspectRatio << " failed: M[0][0] = "
//                       << projMat.M[0][0] << ", expected " << expected << std::endl;
//         }
//     }
//
//     std::cout << "Result: " << (allPassed ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return allPassed;
// }
//
// bool testCase9() {
//     std::cout << "Test Case 9: Z-depth non-linearity test" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 0.1f;
//     float farPlane = 100.0f;
//     float aspectRatio = 1.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Test multiple points at different depths
//     float depths[] = {-nearPlane, -1.0f, -10.0f, -50.0f, -farPlane};
//     bool nonLinear = true;
//     float prevNDCZ = -2.0f; // Start less than -1
//
//     for (int i = 0; i < 5; ++i) {
//         Vector4d point;
//         point.x = 0.0f;
//         point.y = 0.0f;
//         point.z = depths[i];
//         point.w = 1.0f;
//
//         Vector4d projected = projMat * point;
//         Vector4d ndc = perspectiveDivide(projected);
//
//         // Check that NDC Z is increasing with depth (becoming less negative)
//         if (ndc.z <= prevNDCZ) {
//             nonLinear = false;
//             std::cout << "Z-depth not monotonic at depth " << -depths[i]
//                       << ": NDC.z = " << ndc.z << ", previous = " << prevNDCZ << std::endl;
//         }
//         prevNDCZ = ndc.z;
//     }
//
//     std::cout << "Result: " << (nonLinear ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return nonLinear;
// }
//
// bool testCase10() {
//     std::cout << "Test Case 10: Frustum corners projection" << std::endl;
//
//     float fov = 90.0f;
//     float nearPlane = 1.0f;
//     float farPlane = 10.0f;
//     float aspectRatio = 2.0f;
//
//     PerspectiveProjectionMatrix projMat(fov, nearPlane, farPlane, aspectRatio);
//
//     // Calculate frustum corners at near and far planes
//     float fovRad = fov * 3.14159265358979323846f / 180.0f;
//     float tanHalfFov = tan(fovRad * 0.5f);
//
//     float nearRight = nearPlane * tanHalfFov * aspectRatio;
//     float nearTop = nearPlane * tanHalfFov;
//     float farRight = farPlane * tanHalfFov * aspectRatio;
//     float farTop = farPlane * tanHalfFov;
//
//     // Test all 8 frustum corners
//     float corners[8][3] = {
//         {-nearRight, -nearTop, -nearPlane},  // near bottom-left
//         { nearRight, -nearTop, -nearPlane},  // near bottom-right
//         {-nearRight,  nearTop, -nearPlane},  // near top-left
//         { nearRight,  nearTop, -nearPlane},  // near top-right
//         {-farRight,  -farTop,  -farPlane},   // far bottom-left
//         { farRight,  -farTop,  -farPlane},   // far bottom-right
//         {-farRight,   farTop,  -farPlane},   // far top-left
//         { farRight,   farTop,  -farPlane}    // far top-right
//     };
//
//     bool allInNDC = true;
//
//     for (int i = 0; i < 8; ++i) {
//         Vector4d point;
//         point.x = corners[i][0];
//         point.y = corners[i][1];
//         point.z = corners[i][2];
//         point.w = 1.0f;
//
//         Vector4d projected = projMat * point;
//         Vector4d ndc = perspectiveDivide(projected);
//
//         // Check if point is in NDC cube [-1, 1]^3
//         if (ndc.x < -1.01f || ndc.x > 1.01f ||
//             ndc.y < -1.01f || ndc.y > 1.01f ||
//             ndc.z < -1.01f || ndc.z > 1.01f) {
//             allInNDC = false;
//             std::cout << "Corner " << i << " out of NDC bounds: ["
//                       << ndc.x << ", " << ndc.y << ", " << ndc.z << "]" << std::endl;
//         }
//     }
//
//     std::cout << "Result: " << (allInNDC ? "PASSED" : "FAILED") << std::endl << std::endl;
//
//     return allInNDC;
// }
//
// int main() {
//     std::cout << "==================================================" << std::endl;
//     std::cout << "PERSPECTIVE PROJECTION MATRIX TEST SUITE" << std::endl;
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
