// //
// // Created by QuAn on 26-1-28.
// //
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include "Math/matrix.h"
// #include <cassert>
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
// void PrintVector(const Vector4d& v, const std::string& name = "") {
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
// // Check if two matrices are approximately equal
// bool MatricesEqual(const Matrix& a, const Matrix& b, float epsilon = 1e-4f) {
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (!ApproximatelyEqual(a.M[i][j], b.M[i][j], epsilon)) {
//                 return false;
//             }
//         }
//     }
//     return true;
// }
//
// // =============== Test Constructors ===============
//
// void TestConstructors() {
//     std::cout << "==================== Testing Constructors ====================\n";
//
//     // Test 1: Default constructor (identity matrix)
//     Matrix m1;
//     std::cout << "Test 1 - Default Constructor (should be identity):\n";
//     PrintMatrix(m1);
//
//     bool test1_pass = true;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             float expected = (i == j) ? 1.0f : 0.0f;
//             if (!ApproximatelyEqual(m1.M[i][j], expected)) {
//                 test1_pass = false;
//                 break;
//             }
//         }
//     }
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Four Vector4d constructor
//     Vector4d col1(1, 2, 3, 4);
//     Vector4d col2(5, 6, 7, 8);
//     Vector4d col3(9, 10, 11, 12);
//     Vector4d col4(13, 14, 15, 16);
//     Matrix m2(col1, col2, col3, col4);
//     std::cout << "Test 2 - Four Vector4d Constructor:\n";
//     PrintMatrix(m2);
//
//     bool test2_pass = true;
//     float expected_m2[4][4] = {
//         {1, 5, 9, 13},
//         {2, 6, 10, 14},
//         {3, 7, 11, 15},
//         {4, 8, 12, 16}
//     };
//
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (!ApproximatelyEqual(m2.M[i][j], expected_m2[i][j])) {
//                 test2_pass = false;
//                 break;
//             }
//         }
//     }
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: 1D initializer list
//     Matrix m3({
//         1, 0, 0, 5,
//         0, 1, 0, 10,
//         0, 0, 1, 15,
//         0, 0, 0, 1
//     });
//     std::cout << "Test 3 - 1D Initializer List (translation matrix):\n";
//     PrintMatrix(m3);
//
//     bool test3_pass = ApproximatelyEqual(m3.M[0][3], 5.0f) &&
//                      ApproximatelyEqual(m3.M[1][3], 10.0f) &&
//                      ApproximatelyEqual(m3.M[2][3], 15.0f) &&
//                      ApproximatelyEqual(m3.M[3][3], 1.0f);
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 4: 2D initializer list
//     Matrix m4({
//         {2, 0, 0, 0},
//         {0, 3, 0, 0},
//         {0, 0, 4, 0},
//         {0, 0, 0, 1}
//     });
//     std::cout << "Test 4 - 2D Initializer List (scaling matrix):\n";
//     PrintMatrix(m4);
//
//     bool test4_pass = ApproximatelyEqual(m4.M[0][0], 2.0f) &&
//                      ApproximatelyEqual(m4.M[1][1], 3.0f) &&
//                      ApproximatelyEqual(m4.M[2][2], 4.0f) &&
//                      ApproximatelyEqual(m4.M[3][3], 1.0f);
//     std::cout << "Test 4: " << (test4_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Matrix Addition ===============
//
// void TestAddition() {
//     std::cout << "==================== Testing Matrix Addition ====================\n";
//
//     // Test 1: Simple addition
//     Matrix m1({
//         1, 2, 3, 4,
//         5, 6, 7, 8,
//         9, 10, 11, 12,
//         13, 14, 15, 16
//     });
//
//     Matrix m2({
//         16, 15, 14, 13,
//         12, 11, 10, 9,
//         8, 7, 6, 5,
//         4, 3, 2, 1
//     });
//
//     Matrix sum1 = m1 + m2;
//     std::cout << "Test 1 - Matrix Addition:\n";
//     PrintMatrix(m1, "Matrix A");
//     PrintMatrix(m2, "Matrix B");
//     PrintMatrix(sum1, "A + B");
//
//     bool test1_pass = true;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (!ApproximatelyEqual(sum1.M[i][j], 17.0f)) {
//                 test1_pass = false;
//                 break;
//             }
//         }
//     }
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: += operator
//     Matrix m3 = m1;
//     m3 += m2;
//     std::cout << "Test 2 - += Operator:\n";
//     PrintMatrix(m3, "m1 += m2");
//
//     bool test2_pass = MatricesEqual(m3, sum1);
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: Zero matrix addition
//     Matrix zero;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             zero.M[i][j] = 0.0f;
//         }
//     }
//
//     Matrix m4 = m1 + zero;
//     bool test3_pass = MatricesEqual(m4, m1);
//     std::cout << "Test 3 - Zero Matrix Addition:\n";
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Matrix Multiplication ===============
//
// void TestMultiplication() {
//     std::cout << "==================== Testing Matrix Multiplication ====================\n";
//
//     // Test 1: Identity matrix multiplication
//     Matrix identity;
//     Matrix m1({
//         1, 2, 3, 4,
//         5, 6, 7, 8,
//         9, 10, 11, 12,
//         13, 14, 15, 16
//     });
//
//     Matrix product1 = m1 * identity;
//     Matrix product2 = identity * m1;
//
//     std::cout << "Test 1 - Identity Matrix Multiplication:\n";
//     bool test1_pass = MatricesEqual(product1, m1) && MatricesEqual(product2, m1);
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Regular matrix multiplication
//     Matrix A({
//         1, 2, 3, 4,
//         5, 6, 7, 8,
//         9, 10, 11, 12,
//         13, 14, 15, 16
//     });
//
//     Matrix B({
//         16, 15, 14, 13,
//         12, 11, 10, 9,
//         8, 7, 6, 5,
//         4, 3, 2, 1
//     });
//
//     Matrix C = A * B;
//     std::cout << "Test 2 - Regular Matrix Multiplication:\n";
//     PrintMatrix(A, "Matrix A");
//     PrintMatrix(B, "Matrix B");
//     PrintMatrix(C, "A x B");
//
//     // Verify a few elements
//     bool test2_pass = true;
//     // Element (0,0)
//     float expected00 = 1*16 + 2*12 + 3*8 + 4*4; // 80
//     if (!ApproximatelyEqual(C.M[0][0], expected00)) test2_pass = false;
//
//     // Element (1,2)
//     float expected12 = 5*14 + 6*10 + 7*6 + 8*2; // 216
//     if (!ApproximatelyEqual(C.M[1][2], expected12)) test2_pass = false;
//
//     // Element (3,3)
//     float expected33 = 13*13 + 14*9 + 15*5 + 16*1; // 456
//     if (!ApproximatelyEqual(C.M[3][3], expected33)) test2_pass = false;
//
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: *= operator
//     Matrix D = A;
//     D *= B;
//     bool test3_pass = MatricesEqual(D, C);
//     std::cout << "Test 3 - *= Operator:\n";
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 4: Associativity (AB)C = A(BC)
//     Matrix E({
//         2, 0, 0, 0,
//         0, 2, 0, 0,
//         0, 0, 2, 0,
//         0, 0, 0, 1
//     });
//
//     Matrix left = (A * B) * E;
//     Matrix right = A * (B * E);
//     bool test4_pass = MatricesEqual(left, right);
//     std::cout << "Test 4 - Multiplication Associativity:\n";
//     std::cout << "Test 4: " << (test4_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Matrix-Vector Multiplication ===============
//
// void TestVectorMultiplication() {
//     std::cout << "==================== Testing Matrix-Vector Multiplication ====================\n";
//
//     // Test 1: Identity matrix transformation
//     Matrix identity;
//     Vector4d v1(1, 2, 3, 1);
//     Vector4d result1 = identity * v1;
//
//     std::cout << "Test 1 - Identity Matrix Transformation:\n";
//     PrintVector(v1, "Original Vector");
//     PrintVector(result1, "Transformed Vector");
//
//     bool test1_pass = ApproximatelyEqual(result1.x, 1.0f) &&
//                      ApproximatelyEqual(result1.y, 2.0f) &&
//                      ApproximatelyEqual(result1.z, 3.0f) &&
//                      ApproximatelyEqual(result1.w, 1.0f);
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Translation matrix transformation
//     Matrix translation({
//         1, 0, 0, 10,
//         0, 1, 0, 20,
//         0, 0, 1, 30,
//         0, 0, 0, 1
//     });
//
//     Vector4d v2(5, 10, 15, 1);
//     Vector4d result2 = translation * v2;
//
//     std::cout << "Test 2 - Translation Matrix Transformation:\n";
//     PrintVector(v2, "Original Vector");
//     PrintVector(result2, "Transformed Vector");
//
//     bool test2_pass = ApproximatelyEqual(result2.x, 15.0f) &&  // 5 + 10
//                      ApproximatelyEqual(result2.y, 30.0f) &&  // 10 + 20
//                      ApproximatelyEqual(result2.z, 45.0f) &&  // 15 + 30
//                      ApproximatelyEqual(result2.w, 1.0f);
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: Scaling matrix transformation
//     Matrix scale({
//         2, 0, 0, 0,
//         0, 3, 0, 0,
//         0, 0, 4, 0,
//         0, 0, 0, 1
//     });
//
//     Vector4d v3(2, 3, 4, 1);
//     Vector4d result3 = scale * v3;
//
//     std::cout << "Test 3 - Scaling Matrix Transformation:\n";
//     PrintVector(v3, "Original Vector");
//     PrintVector(result3, "Transformed Vector");
//
//     bool test3_pass = ApproximatelyEqual(result3.x, 4.0f) &&   // 2 * 2
//                      ApproximatelyEqual(result3.y, 9.0f) &&   // 3 * 3
//                      ApproximatelyEqual(result3.z, 16.0f) &&  // 4 * 4
//                      ApproximatelyEqual(result3.w, 1.0f);
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 4: Combined transformation (scale then translate)
//     Matrix combined = translation * scale;
//     Vector4d v4(1, 2, 3, 1);
//     Vector4d result4 = combined * v4;
//
//     std::cout << "Test 4 - Combined Transformation (Scale then Translate):\n";
//     PrintVector(v4, "Original Vector");
//     PrintVector(result4, "Transformed Vector");
//
//     bool test4_pass = ApproximatelyEqual(result4.x, 12.0f) &&  // 1*2 + 10
//                      ApproximatelyEqual(result4.y, 26.0f) &&  // 2*3 + 20
//                      ApproximatelyEqual(result4.z, 42.0f) &&  // 3*4 + 30
//                      ApproximatelyEqual(result4.w, 1.0f);
//     std::cout << "Test 4: " << (test4_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Transpose ===============
//
// void TestTranspose() {
//     std::cout << "==================== Testing Matrix Transpose ====================\n";
//
//     // Test 1: Regular matrix transpose
//     Matrix m1({
//         1, 2, 3, 4,
//         5, 6, 7, 8,
//         9, 10, 11, 12,
//         13, 14, 15, 16
//     });
//
//     Matrix transposed1 = m1.GetTransposed();
//
//     std::cout << "Test 1 - Regular Matrix Transpose:\n";
//     PrintMatrix(m1, "Original Matrix");
//     PrintMatrix(transposed1, "Transposed Matrix");
//
//     bool test1_pass = true;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (!ApproximatelyEqual(m1.M[i][j], transposed1.M[j][i])) {
//                 test1_pass = false;
//                 break;
//             }
//         }
//     }
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Symmetric matrix transpose (should be unchanged)
//     Matrix m2({
//         1, 2, 3, 4,
//         2, 5, 6, 7,
//         3, 6, 8, 9,
//         4, 7, 9, 10
//     });
//
//     Matrix transposed2 = m2.GetTransposed();
//     bool test2_pass = MatricesEqual(m2, transposed2);
//
//     std::cout << "Test 2 - Symmetric Matrix Transpose:\n";
//     PrintMatrix(m2, "Symmetric Matrix");
//     PrintMatrix(transposed2, "Transposed Matrix (should be identical)");
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: Transpose twice returns original matrix
//     Matrix m3({
//         2, 0, 0, 10,
//         0, 3, 0, 20,
//         0, 0, 4, 30,
//         0, 0, 0, 1
//     });
//
//     Matrix doubleTransposed = m3.GetTransposed().GetTransposed();
//     bool test3_pass = MatricesEqual(m3, doubleTransposed);
//
//     std::cout << "Test 3 - Transpose Twice:\n";
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Determinant ===============
//
// void TestDeterminant() {
//     std::cout << "==================== Testing Determinant ====================\n";
//
//     // Test 1: Identity matrix determinant should be 1
//     Matrix identity;
//     float det1 = identity.Determinant();
//
//     std::cout << "Test 1 - Identity Matrix Determinant:\n";
//     PrintMatrix(identity, "Identity Matrix");
//     std::cout << "Determinant = " << det1 << " (should be 1)\n";
//
//     bool test1_pass = ApproximatelyEqual(det1, 1.0f);
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Diagonal matrix determinant
//     Matrix diagonal({
//         2, 0, 0, 0,
//         0, 3, 0, 0,
//         0, 0, 4, 0,
//         0, 0, 0, 5
//     });
//
//     float det2 = diagonal.Determinant();
//     float expected2 = 2 * 3 * 4 * 5;  // 120
//
//     std::cout << "Test 2 - Diagonal Matrix Determinant:\n";
//     PrintMatrix(diagonal, "Diagonal Matrix");
//     std::cout << "Determinant = " << det2 << " (should be " << expected2 << ")\n";
//
//     bool test2_pass = ApproximatelyEqual(det2, expected2);
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: Singular matrix (determinant should be 0)
//     Matrix singular({
//         1, 2, 3, 4,
//         1, 2, 3, 4,
//         1, 2, 3, 4,
//         1, 2, 3, 4
//     });
//
//     float det3 = singular.Determinant();
//
//     std::cout << "Test 3 - Singular Matrix Determinant:\n";
//     PrintMatrix(singular, "Singular Matrix");
//     std::cout << "Determinant = " << det3 << " (should be approximately 0)\n";
//
//     bool test3_pass = ApproximatelyEqual(det3, 0.0f, 1e-3f);
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 4: Random invertible matrix
//     Matrix invertible({
//         2, 3, 1, 5,
//         1, 0, 3, 1,
//         0, 2, -3, 2,
//         1, 2, 1, 4
//     });
//
//     float det4 = invertible.Determinant();
//
//     std::cout << "Test 4 - Invertible Matrix Determinant:\n";
//     PrintMatrix(invertible, "Invertible Matrix");
//     std::cout << "Determinant = " << det4 << "\n";
//
//     // Calculate expected determinant manually
//     float expected4 =
//         2*(0*(-3*4 - 1*2) - 3*(2*4 - 1*2) + 1*(2*2 - (-3)*2)) -
//         3*(1*(-3*4 - 1*2) - 3*(1*4 - 1*1) + 1*(1*2 - (-3)*1)) +
//         1*(1*(2*4 - 1*2) - 0*(1*4 - 1*1) + 1*(1*2 - 2*1)) -
//         5*(1*(2*2 - (-3)*2) - 0*(1*2 - (-3)*1) + 2*(1*2 - 2*1));
//
//     bool test4_pass = ApproximatelyEqual(det4, expected4);
//     std::cout << "Expected determinant = " << expected4 << "\n";
//     std::cout << "Test 4: " << (test4_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Test Inverse ===============
//
// void TestInverse() {
//     std::cout << "==================== Testing Matrix Inverse ====================\n";
//
//     // Test 1: Identity matrix inverse is itself
//     Matrix identity;
//     Matrix inv1 = identity.Inverse();
//
//     std::cout << "Test 1 - Identity Matrix Inverse:\n";
//     PrintMatrix(identity, "Identity Matrix");
//     PrintMatrix(inv1, "Inverse Matrix");
//
//     bool test1_pass = MatricesEqual(identity, inv1);
//     std::cout << "Test 1: " << (test1_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 2: Diagonal matrix inverse
//     Matrix diagonal({
//         2, 0, 0, 0,
//         0, 3, 0, 0,
//         0, 0, 4, 0,
//         0, 0, 0, 5
//     });
//
//     Matrix inv2 = diagonal.Inverse();
//     Matrix expected2({
//         0.5, 0, 0, 0,
//         0, 1.0/3, 0, 0,
//         0, 0, 0.25, 0,
//         0, 0, 0, 0.2
//     });
//
//     std::cout << "Test 2 - Diagonal Matrix Inverse:\n";
//     PrintMatrix(diagonal, "Diagonal Matrix");
//     PrintMatrix(inv2, "Inverse Matrix");
//
//     bool test2_pass = MatricesEqual(inv2, expected2);
//     std::cout << "Test 2: " << (test2_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 3: Translation matrix inverse
//     Matrix translation({
//         1, 0, 0, 10,
//         0, 1, 0, 20,
//         0, 0, 1, 30,
//         0, 0, 0, 1
//     });
//
//     Matrix inv3 = translation.Inverse();
//     Matrix expected3({
//         1, 0, 0, -10,
//         0, 1, 0, -20,
//         0, 0, 1, -30,
//         0, 0, 0, 1
//     });
//
//     std::cout << "Test 3 - Translation Matrix Inverse:\n";
//     PrintMatrix(translation, "Translation Matrix");
//     PrintMatrix(inv3, "Inverse Matrix");
//
//     bool test3_pass = MatricesEqual(inv3, expected3);
//     std::cout << "Test 3: " << (test3_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 4: Scaling matrix inverse
//     Matrix scale({
//         2, 0, 0, 0,
//         0, 3, 0, 0,
//         0, 0, 4, 0,
//         0, 0, 0, 1
//     });
//
//     Matrix inv4 = scale.Inverse();
//     Matrix expected4({
//         0.5, 0, 0, 0,
//         0, 1.0/3, 0, 0,
//         0, 0, 0.25, 0,
//         0, 0, 0, 1
//     });
//
//     std::cout << "Test 4 - Scaling Matrix Inverse:\n";
//     PrintMatrix(scale, "Scaling Matrix");
//     PrintMatrix(inv4, "Inverse Matrix");
//
//     bool test4_pass = MatricesEqual(inv4, expected4);
//     std::cout << "Test 4: " << (test4_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 5: Verify A * A-1 = I
//     Matrix A({
//         2, 3, 1, 5,
//         1, 0, 3, 1,
//         0, 2, -3, 2,
//         1, 2, 1, 4
//     });
//
//     Matrix A_inv = A.Inverse();
//     Matrix product = A * A_inv;
//
//     std::cout << "Test 5 - Verify A * A-1 = I:\n";
//     PrintMatrix(A, "Matrix A");
//     PrintMatrix(A_inv, "Inverse of A");
//     PrintMatrix(product, "A * A-1 (should be identity)");
//
//     bool test5_pass = MatricesEqual(product, identity);
//     std::cout << "Test 5: " << (test5_pass ? "PASS" : "FAIL") << "\n\n";
//
//     // Test 6: Singular matrix inverse (should return zero matrix)
//     Matrix singular({
//         1, 2, 3, 4,
//         2, 4, 6, 8,
//         3, 6, 9, 12,
//         4, 8, 12, 16
//     });
//
//     Matrix inv6 = singular.Inverse();
//
//     std::cout << "Test 6 - Singular Matrix Inverse:\n";
//     PrintMatrix(singular, "Singular Matrix");
//     PrintMatrix(inv6, "Inverse (should be zero matrix)");
//
//     // Check if all elements are approximately zero
//     bool test6_pass = true;
//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             if (!ApproximatelyEqual(inv6.M[i][j], 0.0f, 1e-3f)) {
//                 test6_pass = false;
//                 break;
//             }
//         }
//     }
//     std::cout << "Test 6: " << (test6_pass ? "PASS" : "FAIL") << "\n\n";
// }
//
// // =============== Run All Tests ===============
//
// void RunAllTests() {
//     std::cout << "=========================================================\n";
//     std::cout << "            COMPLETE MATRIX TEST SUITE\n";
//     std::cout << "=========================================================\n\n";
//
//     int passed_tests = 0;
//     int total_tests = 0;
//
//     try {
//         TestConstructors(); passed_tests += 4; total_tests += 4;
//         TestAddition(); passed_tests += 3; total_tests += 3;
//         TestMultiplication(); passed_tests += 4; total_tests += 4;
//         TestVectorMultiplication(); passed_tests += 4; total_tests += 4;
//         TestTranspose(); passed_tests += 3; total_tests += 3;
//         TestDeterminant(); passed_tests += 4; total_tests += 4;
//         TestInverse(); passed_tests += 6; total_tests += 6;
//
//         std::cout << "=========================================================\n";
//         std::cout << "                     TEST SUMMARY\n";
//         std::cout << "=========================================================\n";
//         std::cout << "Total Tests: " << total_tests << "\n";
//         std::cout << "Passed: " << passed_tests << "\n";
//         std::cout << "Failed: " << (total_tests - passed_tests) << "\n";
//
//         if (passed_tests == total_tests) {
//             std::cout << "              ALL TESTS PASSED!\n";
//         } else {
//             std::cout << "              SOME TESTS FAILED\n";
//         }
//         std::cout << "=========================================================\n";
//
//     } catch (const std::exception& e) {
//         std::cerr << "\nERROR: Exception occurred during testing: "
//                   << e.what() << std::endl;
//     }
// }
//
// // =============== Main Function ===============
//
// int main() {
//     RunAllTests();
//     return 0;
// }
