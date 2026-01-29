// //
// // Created by QuAn on 26-1-27.
// //
//
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include "Math/Vector4d.h"
//
// using namespace CS::Math;
//
// // Helper function: compare floating point numbers
// bool FloatEqual(float a, float b, float tolerance = CS_KINDA_SMALL_NUMBER) {
//     return std::fabs(a - b) < tolerance;
// }
//
// // Helper function: print vector
// void PrintVector4d(const Vector4d& v, const std::string& name = "Vector4d") {
//     std::cout << std::fixed << std::setprecision(6);
//     std::cout << name << ": (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
// }
//
// // Helper function: print test result
// void PrintTestResult(bool passed, const std::string& testName) {
//     std::cout << testName << ": "
//               << (passed ? "PASSED" : "FAILED")
//               << std::endl;
// }
//
// int main() {
//     std::cout << "=== Vector4d Test Program ===" << std::endl;
//     std::cout << std::endl;
//
//     // Test 1: Constructors
//     {
//         std::cout << "Test 1: Constructors" << std::endl;
//         Vector4d v1;  // Default constructor
//         Vector4d v2(1.0f, 2.0f, 3.0f, 4.0f);  // Four-parameter constructor
//         Vector4d v3(5.0f, 6.0f, 7.0f);  // Three-parameter constructor (w defaults to 1)
//         Vector4d v4{8.0f, 9.0f, 10.0f, 11.0f};  // Initializer list constructor
//
//         bool test1 = FloatEqual(v1.x, 0.0f) && FloatEqual(v1.y, 0.0f) &&
//                      FloatEqual(v1.z, 0.0f) && FloatEqual(v1.w, 0.0f);
//         bool test2 = FloatEqual(v2.x, 1.0f) && FloatEqual(v2.y, 2.0f) &&
//                      FloatEqual(v2.z, 3.0f) && FloatEqual(v2.w, 4.0f);
//         bool test3 = FloatEqual(v3.x, 5.0f) && FloatEqual(v3.y, 6.0f) &&
//                      FloatEqual(v3.z, 7.0f) && FloatEqual(v3.w, 1.0f);
//         bool test4 = FloatEqual(v4.x, 8.0f) && FloatEqual(v4.y, 9.0f) &&
//                      FloatEqual(v4.z, 10.0f) && FloatEqual(v4.w, 11.0f);
//
//         PrintTestResult(test1, "Default constructor");
//         PrintTestResult(test2, "Four-parameter constructor");
//         PrintTestResult(test3, "Three-parameter constructor (w defaults to 1)");
//         PrintTestResult(test4, "Initializer list constructor");
//         std::cout << std::endl;
//     }
//
//     // Test 2: Constructor from Vector3d
//     {
//         std::cout << "Test 2: Constructor from Vector3d" << std::endl;
//         Vector3d v3d(1.0f, 2.0f, 3.0f);
//
//         Vector4d v1(v3d);  // w defaults to 1
//         Vector4d v2(v3d, 4.0f);  // w specified as 4
//
//         PrintVector4d(v1, "From Vector3d (default w=1)");
//         PrintVector4d(v2, "From Vector3d (w=4)");
//
//         bool test1 = FloatEqual(v1.x, 1.0f) && FloatEqual(v1.y, 2.0f) &&
//                      FloatEqual(v1.z, 3.0f) && FloatEqual(v1.w, 1.0f);
//         bool test2 = FloatEqual(v2.x, 1.0f) && FloatEqual(v2.y, 2.0f) &&
//                      FloatEqual(v2.z, 3.0f) && FloatEqual(v2.w, 4.0f);
//
//         PrintTestResult(test1, "Vector3d constructor with default w");
//         PrintTestResult(test2, "Vector3d constructor with specified w");
//         std::cout << std::endl;
//     }
//
//     // Test 3: Set method
//     {
//         std::cout << "Test 3: Set method" << std::endl;
//         Vector4d v;
//         v.Set(7.0f, 8.0f, 9.0f, 10.0f);
//         bool test = FloatEqual(v.x, 7.0f) && FloatEqual(v.y, 8.0f) &&
//                     FloatEqual(v.z, 9.0f) && FloatEqual(v.w, 10.0f);
//         PrintTestResult(test, "Set method");
//         std::cout << std::endl;
//     }
//
//     // Test 4: Length calculation
//     {
//         std::cout << "Test 4: Length calculation" << std::endl;
//         Vector4d v1(3.0f, 4.0f, 0.0f, 0.0f);  // Length should be 5
//         Vector4d v2(1.0f, 2.0f, 2.0f, 0.0f);  // Length should be 3
//         Vector4d v3(1.0f, 2.0f, 2.0f, 2.0f);  // Length should be sqrt(1+4+4+4) = sqrt(13) ≈ 3.605551
//
//         float len1 = v1.Length();
//         float len2 = v2.Length();
//         float len3 = v3.Length();
//
//         PrintVector4d(v1, "Test vector 1");
//         std::cout << "Calculated length: " << len1 << " (Expected: 5.0)" << std::endl;
//
//         PrintVector4d(v2, "Test vector 2");
//         std::cout << "Calculated length: " << len2 << " (Expected: 3.0)" << std::endl;
//
//         PrintVector4d(v3, "Test vector 3");
//         std::cout << "Calculated length: " << len3 << " (Expected: " << std::sqrt(13.0f) << ")" << std::endl;
//
//         bool test1 = FloatEqual(len1, 5.0f);
//         bool test2 = FloatEqual(len2, 3.0f);
//         bool test3 = FloatEqual(len3, std::sqrt(13.0f));
//
//         PrintTestResult(test1, "Vector 1 length calculation");
//         PrintTestResult(test2, "Vector 2 length calculation");
//         PrintTestResult(test3, "Vector 3 length calculation");
//         std::cout << std::endl;
//     }
//
//     // Test 5: Dot product
//     {
//         std::cout << "Test 5: Dot product" << std::endl;
//         Vector4d v1(1.0f, 2.0f, 3.0f, 4.0f);
//         Vector4d v2(5.0f, 6.0f, 7.0f, 8.0f);
//
//         float dot1 = v1.Dot(v2);
//         float dot2 = Vector4d::DotProduct(v1, v2);
//
//         PrintVector4d(v1, "Vector 1");
//         PrintVector4d(v2, "Vector 2");
//         std::cout << "Dot product (member): " << dot1 << std::endl;
//         std::cout << "Dot product (static): " << dot2 << std::endl;
//         std::cout << "Expected: " << (1*5 + 2*6 + 3*7 + 4*8) << " = 70" << std::endl;
//
//         bool test1 = FloatEqual(dot1, 70.0f);
//         bool test2 = FloatEqual(dot2, 70.0f);
//         bool test3 = FloatEqual(dot1, dot2);
//
//         PrintTestResult(test1, "Member dot product");
//         PrintTestResult(test2, "Static dot product");
//         PrintTestResult(test3, "Both methods consistent");
//         std::cout << std::endl;
//     }
//
//     // Test 6: Normalization
//     {
//         std::cout << "Test 6: Normalization" << std::endl;
//         Vector4d v(3.0f, 4.0f, 0.0f, 0.0f);
//         float originalLength = v.Length();
//
//         PrintVector4d(v, "Original vector");
//         std::cout << "Original length: " << originalLength << std::endl;
//
//         v.Normalize();
//         float normalizedLength = v.Length();
//
//         PrintVector4d(v, "Normalized vector");
//         std::cout << "Normalized length: " << normalizedLength << std::endl;
//
//         bool test1 = FloatEqual(originalLength, 5.0f);
//         bool test2 = FloatEqual(normalizedLength, 1.0f, 0.0001f);
//         bool test3 = FloatEqual(v.x, 0.6f) && FloatEqual(v.y, 0.8f) &&
//                      FloatEqual(v.z, 0.0f) && FloatEqual(v.w, 0.0f);
//
//         PrintTestResult(test1, "Original length correct");
//         PrintTestResult(test2, "Normalized length is 1");
//         PrintTestResult(test3, "Normalized direction correct");
//
//         // Test with non-zero w component
//         Vector4d v2(2.0f, 4.0f, 4.0f, 2.0f);
//         v2.Normalize();
//         float len2 = v2.Length();
//         PrintVector4d(v2, "Normalized vector with non-zero w");
//         std::cout << "Length after normalization: " << len2 << std::endl;
//         bool test4 = FloatEqual(len2, 1.0f, 0.0001f);
//         PrintTestResult(test4, "Vector with non-zero w normalizes correctly");
//         std::cout << std::endl;
//     }
//
//     // Test 7: Vector operations
//     {
//         std::cout << "Test 7: Vector operations" << std::endl;
//         Vector4d a(1.0f, 2.0f, 3.0f, 4.0f);
//         Vector4d b(5.0f, 6.0f, 7.0f, 8.0f);
//
//         // Addition
//         Vector4d sum = a + b;
//         PrintVector4d(a, "Vector A");
//         PrintVector4d(b, "Vector B");
//         PrintVector4d(sum, "A + B");
//         bool testAdd = sum == Vector4d(6.0f, 8.0f, 10.0f, 12.0f);
//         PrintTestResult(testAdd, "Vector addition");
//
//         // Subtraction
//         Vector4d diff = a - b;
//         PrintVector4d(diff, "A - B");
//         bool testSub = diff == Vector4d(-4.0f, -4.0f, -4.0f, -4.0f);
//         PrintTestResult(testSub, "Vector subtraction");
//
//         // Scalar multiplication (right)
//         Vector4d scaled = a * 2.0f;
//         PrintVector4d(scaled, "A * 2.0");
//         bool testMul1 = scaled == Vector4d(2.0f, 4.0f, 6.0f, 8.0f);
//         PrintTestResult(testMul1, "Vector scalar multiplication (right)");
//
//         // Scalar multiplication (left)
//         Vector4d scaled2 = 2.0f * a;
//         PrintVector4d(scaled2, "2.0 * A");
//         bool testMul2 = scaled2 == Vector4d(2.0f, 4.0f, 6.0f, 8.0f);
//         PrintTestResult(testMul2, "Vector scalar multiplication (left)");
//
//         // Division
//         Vector4d divided = scaled / 2.0f;
//         PrintVector4d(divided, "(A * 2.0) / 2.0");
//         bool testDiv = divided == a;
//         PrintTestResult(testDiv, "Vector scalar division");
//
//         std::cout << std::endl;
//     }
//
//     // Test 8: Compound assignment operations
//     {
//         std::cout << "Test 8: Compound assignment operations" << std::endl;
//         Vector4d v(1.0f, 2.0f, 3.0f, 4.0f);
//
//         v += Vector4d(1.0f, 1.0f, 1.0f, 1.0f);
//         PrintVector4d(v, "v += (1,1,1,1)");
//         bool test1 = v == Vector4d(2.0f, 3.0f, 4.0f, 5.0f);
//         PrintTestResult(test1, "+= operation");
//
//         v -= Vector4d(1.0f, 1.0f, 1.0f, 1.0f);
//         PrintVector4d(v, "v -= (1,1,1,1)");
//         bool test2 = v == Vector4d(1.0f, 2.0f, 3.0f, 4.0f);
//         PrintTestResult(test2, "-= operation");
//
//         v *= 2.0f;
//         PrintVector4d(v, "v *= 2.0");
//         bool test3 = v == Vector4d(2.0f, 4.0f, 6.0f, 8.0f);
//         PrintTestResult(test3, "*= operation");
//
//         v /= 2.0f;
//         PrintVector4d(v, "v /= 2.0");
//         bool test4 = v == Vector4d(1.0f, 2.0f, 3.0f, 4.0f);
//         PrintTestResult(test4, "/= operation");
//
//         std::cout << std::endl;
//     }
//
//     // Test 9: Equality comparison
//     {
//         std::cout << "Test 9: Equality comparison" << std::endl;
//         Vector4d v1(1.0f, 2.0f, 3.0f, 4.0f);
//         Vector4d v2(1.000001f, 2.000001f, 3.000001f, 4.000001f);
//         Vector4d v3(1.1f, 2.0f, 3.0f, 4.0f);
//
//         bool test1 = v1 == v1;
//         bool test2 = v1 == v2;  // Using default tolerance
//         bool test3 = v1.Equal(v2, 0.001f);  // Using larger tolerance
//         bool test4 = v1 == v3;
//
//         PrintVector4d(v1, "Vector 1");
//         PrintVector4d(v2, "Vector 2");
//         PrintVector4d(v3, "Vector 3");
//
//         PrintTestResult(test1, "Self comparison");
//         PrintTestResult(!test2, "Tiny difference (default tolerance)");
//         PrintTestResult(test3, "Tiny difference (larger tolerance)");
//         PrintTestResult(!test4, "Significant difference");
//
//         std::cout << std::endl;
//     }
//
//     // Test 10: Homogeneous coordinates test
//     {
//         std::cout << "Test 10: Homogeneous coordinates" << std::endl;
//
//         // Create a Vector3d point
//         Vector3d point3d(2.0f, 3.0f, 4.0f);
//
//         // Convert to homogeneous coordinates (w=1 for points)
//         Vector4d point4d(point3d, 1.0f);
//         PrintVector4d(point4d, "Point in homogeneous coordinates");
//
//         // Test division by w (perspective division)
//         Vector4d projected = point4d / point4d.w;
//         PrintVector4d(projected, "After perspective division");
//
//         // Test w=0 (direction vector)
//         Vector4d direction(point3d, 0.0f);
//         PrintVector4d(direction, "Direction vector (w=0)");
//
//         bool test1 = FloatEqual(point4d.x, 2.0f) && FloatEqual(point4d.y, 3.0f) &&
//                      FloatEqual(point4d.z, 4.0f) && FloatEqual(point4d.w, 1.0f);
//         bool test2 = FloatEqual(projected.x, 2.0f) && FloatEqual(projected.y, 3.0f) &&
//                      FloatEqual(projected.z, 4.0f) && FloatEqual(projected.w, 1.0f);
//         bool test3 = FloatEqual(direction.w, 0.0f);
//
//         PrintTestResult(test1, "Point to homogeneous conversion correct");
//         PrintTestResult(test2, "Perspective division works correctly");
//         PrintTestResult(test3, "Direction vector has w=0");
//
//         std::cout << std::endl;
//     }
//
//     // Test 11: Edge cases
//     {
//         std::cout << "Test 11: Edge cases" << std::endl;
//
//         // Zero vector
//         Vector4d zero(0.0f, 0.0f, 0.0f, 0.0f);
//         PrintVector4d(zero, "Zero vector");
//         std::cout << "Zero vector length: " << zero.Length() << std::endl;
//         bool test1 = FloatEqual(zero.Length(), 0.0f);
//         PrintTestResult(test1, "Zero vector has zero length");
//
//         // Try to normalize zero vector (should handle gracefully)
//         Vector4d zeroCopy = zero;
//         zeroCopy.Normalize();
//         PrintVector4d(zeroCopy, "Zero vector after normalization");
//         bool test2 = FloatEqual(zeroCopy.Length(), 0.0f);
//         PrintTestResult(test2, "Normalized zero vector still has zero length");
//
//         // Vector with very small components
//         Vector4d small(0.0001f, 0.0002f, 0.0003f, 0.0004f);
//         float smallLength = small.Length();
//         std::cout << "Small vector length: " << smallLength << std::endl;
//         bool test3 = smallLength > 0.0f;
//         PrintTestResult(test3, "Small vector has positive length");
//
//         // Normalize small vector
//         small.Normalize();
//         float normalizedSmallLength = small.Length();
//         std::cout << "Normalized small vector length: " << normalizedSmallLength << std::endl;
//         bool test4 = FloatEqual(normalizedSmallLength, 1.0f, 0.0001f);
//         PrintTestResult(test4, "Small vector normalizes to unit length");
//
//         std::cout << std::endl;
//     }
//
//     // Test 12: Consistency with Vector3d operations
//     {
//         std::cout << "Test 12: Consistency with Vector3d operations" << std::endl;
//
//         // Test that Vector4d operations behave like Vector3d when w=0 or w=1
//         Vector3d v3d1(1.0f, 2.0f, 3.0f);
//         Vector3d v3d2(4.0f, 5.0f, 6.0f);
//
//         Vector4d v4d1(v3d1, 1.0f);
//         Vector4d v4d2(v3d2, 1.0f);
//
//         // Test addition
//         Vector3d sum3d = v3d1 + v3d2;
//         Vector4d sum4d = v4d1 + v4d2;
//         Vector4d expectedSum(sum3d, 2.0f);  // w components add: 1 + 1 = 2
//
//         PrintVector4d(sum4d, "Vector4d sum");
//         PrintVector4d(expectedSum, "Expected sum");
//         bool test1 = sum4d == expectedSum;
//         PrintTestResult(test1, "Addition with w=1 vectors");
//
//         // Test dot product (ignoring w component when w=0)
//         Vector4d dir1(v3d1, 0.0f);
//         Vector4d dir2(v3d2, 0.0f);
//         float dot3d = v3d1.Dot(v3d2);
//         float dot4d = dir1.Dot(dir2);
//
//         std::cout << "Vector3d dot product: " << dot3d << std::endl;
//         std::cout << "Vector4d dot product (w=0): " << dot4d << std::endl;
//         bool test2 = FloatEqual(dot3d, dot4d);
//         PrintTestResult(test2, "Dot product consistency when w=0");
//
//         std::cout << std::endl;
//     }
//
//     std::cout << "=== All Vector4d tests completed ===" << std::endl;
//
//     return 0;
// }
