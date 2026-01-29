// //
// // Created by QuAn on 26-1-29.
// //
// #include "Math//matrix.h"
// #include "Math/scale_matrix.h"
// #include <iostream>
// #include <iomanip>
// #include <cmath>
//
// using namespace CS::Math;
//
// // Helper function to print matrix
// void PrintMatrix(const Matrix& mat, const std::string& name = "") {
//     if (!name.empty()) {
//         std::cout << name << ":\n";
//     }
//
//     for (int i = 0; i < 4; ++i) {
//         std::cout << "  ";
//         for (int j = 0; j < 4; ++j) {
//             std::cout << std::fixed << std::setprecision(4)
//                      << std::setw(8) << mat.M[i][j] << " ";
//         }
//         std::cout << "\n";
//     }
//     std::cout << std::endl;
// }
//
// // Helper function to print vector
// void PrintVector3d(const Vector3d& v, const std::string& name = "") {
//     if (!name.empty()) {
//         std::cout << name << ": ";
//     }
//     std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
// }
//
// void PrintVector4d(const Vector4d& v, const std::string& name = "") {
//     if (!name.empty()) {
//         std::cout << name << ": ";
//     }
//     std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
// }
//
// // Check if two floats are approximately equal
// bool ApproximatelyEqual(float a, float b, float epsilon = 1e-4f) {
//     return fabs(a - b) < epsilon;
// }
//
// // =============== Test ScaleMatrix ===============
//
// void TestScaleMatrix() {
//     std::cout << "========================================\n";
//     std::cout << "        Testing ScaleMatrix Class\n";
//     std::cout << "========================================\n\n";
//
//     int passed_tests = 0;
//     int total_tests = 0;
//
//     // Test 1: Uniform scaling
//     std::cout << "Test 1 - Uniform Scaling (scale by 2 in all axes):\n";
//     {
//         total_tests++;
//         Vector3d scale1(2.0f, 2.0f, 2.0f);
//         ScaleMatrix scaleMat1(scale1);
//
//         PrintVector3d(scale1, "Scale vector");
//         PrintMatrix(scaleMat1, "Scale matrix");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat1.M[0][0], 2.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat1.M[1][1], 2.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat1.M[2][2], 2.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat1.M[3][3], 1.0f);
//
//         // Check off-diagonal elements are zero
//         for (int i = 0; i < 4 && test_pass; ++i) {
//             for (int j = 0; j < 4 && test_pass; ++j) {
//                 if (i != j && (i < 3 && j < 3)) {
//                     test_pass = test_pass && ApproximatelyEqual(scaleMat1.M[i][j], 0.0f);
//                 }
//             }
//         }
//
//         std::cout << "Test 1: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 2: Non-uniform scaling
//     std::cout << "Test 2 - Non-uniform Scaling:\n";
//     {
//         total_tests++;
//         Vector3d scale2(2.0f, 3.0f, 4.0f);
//         ScaleMatrix scaleMat2(scale2);
//
//         PrintVector3d(scale2, "Scale vector");
//         PrintMatrix(scaleMat2, "Scale matrix");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat2.M[0][0], 2.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat2.M[1][1], 3.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat2.M[2][2], 4.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat2.M[3][3], 1.0f);
//
//         std::cout << "Test 2: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 3: Vector transformation with scaling
//     std::cout << "Test 3 - Vector Transformation with Scaling:\n";
//     {
//         total_tests++;
//         Vector4d point1(1.0f, 2.0f, 3.0f, 1.0f);  // Point
//         Vector4d vector1(1.0f, 2.0f, 3.0f, 0.0f); // Vector
//
//         ScaleMatrix scaleMat3(Vector3d(2.0f, 3.0f, 4.0f));
//
//         Vector4d scaledPoint = scaleMat3 * point1;
//         Vector4d scaledVector = scaleMat3 * vector1;
//
//         PrintVector4d(point1, "Original point");
//         PrintVector4d(scaledPoint, "Scaled point");
//         PrintVector4d(vector1, "Original vector");
//         PrintVector4d(scaledVector, "Scaled vector");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaledPoint.x, 2.0f);   // 1 * 2
//         test_pass = test_pass && ApproximatelyEqual(scaledPoint.y, 6.0f);   // 2 * 3
//         test_pass = test_pass && ApproximatelyEqual(scaledPoint.z, 12.0f);  // 3 * 4
//         test_pass = test_pass && ApproximatelyEqual(scaledPoint.w, 1.0f);   // w unchanged
//
//         test_pass = test_pass && ApproximatelyEqual(scaledVector.x, 2.0f);   // 1 * 2
//         test_pass = test_pass && ApproximatelyEqual(scaledVector.y, 6.0f);   // 2 * 3
//         test_pass = test_pass && ApproximatelyEqual(scaledVector.z, 12.0f);  // 3 * 4
//         test_pass = test_pass && ApproximatelyEqual(scaledVector.w, 0.0f);   // w unchanged
//
//         std::cout << "Test 3: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 4: Rescaling using Make function
//     std::cout << "Test 4 - Rescaling using Make() function:\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat4(Vector3d(1.0f, 1.0f, 1.0f));
//
//         PrintMatrix(scaleMat4, "Initial scale matrix (identity)");
//
//         // Rescale to different values
//         scaleMat4.Make(Vector3d(5.0f, 6.0f, 7.0f));
//         PrintMatrix(scaleMat4, "After Make() with new scale (5, 6, 7)");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat4.M[0][0], 5.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat4.M[1][1], 6.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat4.M[2][2], 7.0f);
//
//         std::cout << "Test 4: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 5: Another uniform scaling test
//     std::cout << "Test 5 - Uniform Scaling (scale by 0.5 in all axes):\n";
//     {
//         total_tests++;
//         Vector3d scale5(0.5f, 0.5f, 0.5f);
//         ScaleMatrix scaleMat5(scale5);
//
//         PrintVector3d(scale5, "Scale vector");
//         PrintMatrix(scaleMat5, "Scale matrix");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat5.M[0][0], 0.5f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat5.M[1][1], 0.5f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat5.M[2][2], 0.5f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat5.M[3][3], 1.0f);
//
//         std::cout << "Test 5: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 6: Combining scaling with translation
//     std::cout << "Test 6 - Combining Scaling with Translation:\n";
//     {
//         total_tests++;
//
//         // Create translation matrix
//         Matrix translation;
//         translation.M[0][0] = 1.0f; translation.M[0][1] = 0.0f; translation.M[0][2] = 0.0f; translation.M[0][3] = 10.0f;
//         translation.M[1][0] = 0.0f; translation.M[1][1] = 1.0f; translation.M[1][2] = 0.0f; translation.M[1][3] = 20.0f;
//         translation.M[2][0] = 0.0f; translation.M[2][1] = 0.0f; translation.M[2][2] = 1.0f; translation.M[2][3] = 30.0f;
//         translation.M[3][0] = 0.0f; translation.M[3][1] = 0.0f; translation.M[3][2] = 0.0f; translation.M[3][3] = 1.0f;
//
//         // Create scale matrix
//         ScaleMatrix scaleMat6(Vector3d(2.0f, 3.0f, 4.0f));
//
//         // Combine transformations: scale then translate
//         Matrix combined = translation * scaleMat6;
//
//         PrintMatrix(translation, "Translation matrix");
//         PrintMatrix(scaleMat6, "Scale matrix");
//         PrintMatrix(combined, "Combined matrix (translation * scale)");
//
//         // Test a point
//         Vector4d point6(1.0f, 2.0f, 3.0f, 1.0f);
//         Vector4d transformed = combined * point6;
//
//         PrintVector4d(point6, "Original point");
//         PrintVector4d(transformed, "Transformed point (scaled then translated)");
//
//         bool test_pass = true;
//         // First scale: (1*2, 2*3, 3*4) = (2, 6, 12)
//         // Then translate: (2+10, 6+20, 12+30) = (12, 26, 42)
//         test_pass = test_pass && ApproximatelyEqual(transformed.x, 12.0f);
//         test_pass = test_pass && ApproximatelyEqual(transformed.y, 26.0f);
//         test_pass = test_pass && ApproximatelyEqual(transformed.z, 42.0f);
//         test_pass = test_pass && ApproximatelyEqual(transformed.w, 1.0f);
//
//         std::cout << "Test 6: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 7: Inverse of scaling matrix
//     std::cout << "Test 7 - Inverse of Scaling Matrix:\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat7(Vector3d(2.0f, 3.0f, 4.0f));
//         Matrix inverseScale = scaleMat7.Inverse();
//
//         PrintMatrix(scaleMat7, "Original scale matrix");
//         PrintMatrix(inverseScale, "Inverse scale matrix");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[0][0], 0.5f);       // 1/2
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[1][1], 1.0f/3.0f);   // 1/3
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[2][2], 0.25f);      // 1/4
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[3][3], 1.0f);
//
//         // Verify original matrix times inverse equals identity
//         Matrix identityCheck = scaleMat7 * inverseScale;
//         bool isIdentity = true;
//         for (int i = 0; i < 4 && isIdentity; ++i) {
//             for (int j = 0; j < 4 && isIdentity; ++j) {
//                 float expected = (i == j) ? 1.0f : 0.0f;
//                 if (!ApproximatelyEqual(identityCheck.M[i][j], expected)) {
//                     isIdentity = false;
//                 }
//             }
//         }
//
//         test_pass = test_pass && isIdentity;
//         std::cout << "Test 7: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 8: Another inverse test with different scales
//     std::cout << "Test 8 - Inverse of Scaling Matrix (different scales):\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat8(Vector3d(0.5f, 2.0f, 8.0f));
//         Matrix inverseScale = scaleMat8.Inverse();
//
//         PrintMatrix(scaleMat8, "Original scale matrix");
//         PrintMatrix(inverseScale, "Inverse scale matrix");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[0][0], 2.0f);   // 1/0.5
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[1][1], 0.5f);   // 1/2
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[2][2], 0.125f); // 1/8
//         test_pass = test_pass && ApproximatelyEqual(inverseScale.M[3][3], 1.0f);
//
//         std::cout << "Test 8: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 9: Determinant of scaling matrix
//     std::cout << "Test 9 - Determinant of Scaling Matrix:\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat9(Vector3d(2.0f, 3.0f, 4.0f));
//         float det = scaleMat9.Determinant();
//
//         PrintMatrix(scaleMat9, "Scale matrix");
//         std::cout << "Determinant = " << det << "\n";
//         std::cout << "Expected = 2 * 3 * 4 = " << (2.0f * 3.0f * 4.0f) << "\n";
//
//         bool test_pass = ApproximatelyEqual(det, 2.0f * 3.0f * 4.0f);
//         std::cout << "Test 9: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 10: Determinant of another scaling matrix
//     std::cout << "Test 10 - Determinant of Scaling Matrix (different scales):\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat10(Vector3d(0.5f, 2.0f, 8.0f));
//         float det = scaleMat10.Determinant();
//
//         PrintMatrix(scaleMat10, "Scale matrix");
//         std::cout << "Determinant = " << det << "\n";
//         std::cout << "Expected = 0.5 * 2 * 8 = " << (0.5f * 2.0f * 8.0f) << "\n";
//
//         bool test_pass = ApproximatelyEqual(det, 0.5f * 2.0f * 8.0f);
//         std::cout << "Test 10: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 11: Transpose of scaling matrix (should be symmetric)
//     std::cout << "Test 11 - Transpose of Scaling Matrix:\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat11(Vector3d(2.0f, 3.0f, 4.0f));
//         Matrix transposed = scaleMat11.GetTransposed();
//
//         PrintMatrix(scaleMat11, "Original scale matrix");
//         PrintMatrix(transposed, "Transposed scale matrix");
//
//         bool test_pass = true;
//         for (int i = 0; i < 4 && test_pass; ++i) {
//             for (int j = 0; j < 4 && test_pass; ++j) {
//                 if (!ApproximatelyEqual(scaleMat11.M[i][j], transposed.M[i][j])) {
//                     test_pass = false;
//                 }
//             }
//         }
//
//         std::cout << "Test 11: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 12: Another transpose test
//     std::cout << "Test 12 - Transpose of Another Scaling Matrix:\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat12(Vector3d(5.0f, 6.0f, 7.0f));
//         Matrix transposed = scaleMat12.GetTransposed();
//
//         PrintMatrix(scaleMat12, "Original scale matrix");
//         PrintMatrix(transposed, "Transposed scale matrix");
//
//         bool test_pass = true;
//         for (int i = 0; i < 4 && test_pass; ++i) {
//             for (int j = 0; j < 4 && test_pass; ++j) {
//                 if (!ApproximatelyEqual(scaleMat12.M[i][j], transposed.M[i][j])) {
//                     test_pass = false;
//                 }
//             }
//         }
//
//         std::cout << "Test 12: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 13: Make function with zero scale (edge case)
//     std::cout << "Test 13 - Make function with zero scale (edge case):\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat13(Vector3d(1.0f, 1.0f, 1.0f));
//         scaleMat13.Make(Vector3d(0.0f, 0.0f, 0.0f));
//
//         PrintMatrix(scaleMat13, "Scale matrix with zero scale");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat13.M[0][0], 0.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat13.M[1][1], 0.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat13.M[2][2], 0.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat13.M[3][3], 1.0f);
//
//         std::cout << "Test 13: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test 14: Negative scaling
//     std::cout << "Test 14 - Negative Scaling (reflection):\n";
//     {
//         total_tests++;
//         ScaleMatrix scaleMat14(Vector3d(-1.0f, 1.0f, -2.0f));
//
//         PrintMatrix(scaleMat14, "Scale matrix with negative components");
//
//         bool test_pass = true;
//         test_pass = test_pass && ApproximatelyEqual(scaleMat14.M[0][0], -1.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat14.M[1][1], 1.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat14.M[2][2], -2.0f);
//         test_pass = test_pass && ApproximatelyEqual(scaleMat14.M[3][3], 1.0f);
//
//         // Test transformation
//         Vector4d point14(2.0f, 3.0f, 4.0f, 1.0f);
//         Vector4d transformed14 = scaleMat14 * point14;
//
//         PrintVector4d(point14, "Original point");
//         PrintVector4d(transformed14, "Transformed point");
//
//         test_pass = test_pass && ApproximatelyEqual(transformed14.x, -2.0f);  // 2 * -1
//         test_pass = test_pass && ApproximatelyEqual(transformed14.y, 3.0f);   // 3 * 1
//         test_pass = test_pass && ApproximatelyEqual(transformed14.z, -8.0f);  // 4 * -2
//         test_pass = test_pass && ApproximatelyEqual(transformed14.w, 1.0f);
//
//         std::cout << "Test 14: " << (test_pass ? "PASS" : "FAIL") << "\n\n";
//         if (test_pass) passed_tests++;
//     }
//
//     // Test Summary
//     std::cout << "========================================\n";
//     std::cout << "              TEST SUMMARY\n";
//     std::cout << "========================================\n";
//     std::cout << "Total tests: " << total_tests << "\n";
//     std::cout << "Passed: " << passed_tests << "\n";
//     std::cout << "Failed: " << (total_tests - passed_tests) << "\n";
//
//     if (passed_tests == total_tests) {
//         std::cout << "           ALL TESTS PASSED\n";
//     } else {
//         std::cout << "           SOME TESTS FAILED\n";
//     }
//     std::cout << "========================================\n";
// }
//
// // =============== Main Function ===============
//
// int main() {
//     TestScaleMatrix();
//     return 0;
// }
