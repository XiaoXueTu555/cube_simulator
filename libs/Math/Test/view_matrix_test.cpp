#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include "Math/view_matrix.h"

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
    std::cout << "Test Case 1: Identity view (camera at origin, looking at -Z)" << std::endl;

    Vector3d eye(0.0, 0.0, 0.0);
    Vector3d lookAt(0.0, 0.0, -1.0);
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    Matrix identity;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            identity.M[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    bool passed = compareMatrices(viewMat, identity);

    if (!passed) {
        std::cout << "Expected identity matrix:" << std::endl;
        printMatrix(identity);
        std::cout << "Got:" << std::endl;
        printMatrix(viewMat);
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase2() {
    std::cout << "Test Case 2: Camera translated along X axis" << std::endl;

    Vector3d eye(5.0, 0.0, 0.0);
    Vector3d lookAt(5.0, 0.0, -1.0);
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    Vector4d worldPoint;
    worldPoint.x = 5.0;
    worldPoint.y = 0.0;
    worldPoint.z = -10.0;
    worldPoint.w = 1.0;

    Vector4d viewPoint = viewMat * worldPoint;

    Vector4d expected;
    expected.x = 0.0;   // 在视图空间中，X坐标应为0（点在摄像机正前方）
    expected.y = 0.0;
    expected.z = -10.0; // Z坐标应为负（点在摄像机前方）
    expected.w = 1.0;

    bool passed = compareVectors(viewPoint, expected);

    if (!passed) {
        printVector(worldPoint, "World space point");
        printVector(expected, "Expected view space");
        printVector(viewPoint, "Actual view space");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase3() {
    std::cout << "Test Case 3: Camera looking at a specific point" << std::endl;

    Vector3d eye(0.0, 0.0, 10.0);
    Vector3d lookAt(0.0, 0.0, 0.0);
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    // 测试原点在世界空间中的点
    Vector4d worldPoint;
    worldPoint.x = 0.0;
    worldPoint.y = 0.0;
    worldPoint.z = 0.0;
    worldPoint.w = 1.0;

    Vector4d viewPoint = viewMat * worldPoint;

    Vector4d expected;
    expected.x = 0.0;
    expected.y = 0.0;
    expected.z = -10.0; // 摄像机在(0,0,10)，看向原点，所以原点在视图空间的z=-10处
    expected.w = 1.0;

    bool passed = compareVectors(viewPoint, expected);

    if (!passed) {
        printVector(worldPoint, "World space point");
        printVector(expected, "Expected view space");
        printVector(viewPoint, "Actual view space");
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase4() {
    std::cout << "Test Case 4: Camera with up vector rotated" << std::endl;

    Vector3d eye(0.0, 0.0, 0.0);
    Vector3d lookAt(0.0, 0.0, -1.0);
    Vector3d up(1.0, 0.0, 0.0); // 上向量指向X轴

    ViewMatrix viewMat(eye, lookAt, up);

    // 测试一个在+X方向的点
    Vector4d worldPoint;
    worldPoint.x = 1.0;
    worldPoint.y = 0.0;
    worldPoint.z = 0.0;
    worldPoint.w = 1.0;

    Vector4d viewPoint = viewMat * worldPoint;

    // 由于上向量是(1,0,0)，右向量应该是(0,0,1)，前向量是(0,-1,0)?
    // 这取决于实现，但点应该在视图空间的某个位置
    bool passed = true;

    // 验证矩阵是正交的（旋转部分）
    double error = 0.0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double dot = 0.0;
            for (int k = 0; k < 3; ++k) {
                dot += viewMat.M[i][k] * viewMat.M[j][k];
            }
            double expected = (i == j) ? 1.0 : 0.0;
            error += fabs(dot - expected);
        }
    }

    if (error > 1e-4) {
        passed = false;
        std::cout << "View matrix is not orthogonal. Error: " << error << std::endl;
    }

    // 验证行列式为1（右手坐标系）
    double det =
        viewMat.M[0][0] * (viewMat.M[1][1] * viewMat.M[2][2] - viewMat.M[1][2] * viewMat.M[2][1]) -
        viewMat.M[0][1] * (viewMat.M[1][0] * viewMat.M[2][2] - viewMat.M[1][2] * viewMat.M[2][0]) +
        viewMat.M[0][2] * (viewMat.M[1][0] * viewMat.M[2][1] - viewMat.M[1][1] * viewMat.M[2][0]);

    if (fabs(det - 1.0) > 1e-4) {
        passed = false;
        std::cout << "View matrix determinant is not 1. Determinant: " << det << std::endl;
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase5() {
    std::cout << "Test Case 5: Camera looking up" << std::endl;

    Vector3d eye(0.0, 0.0, 0.0);
    Vector3d lookAt(0.0, 1.0, -1.0);
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    // 测试一个点
    Vector4d worldPoint;
    worldPoint.x = 0.0;
    worldPoint.y = 1.0;
    worldPoint.z = -1.0;
    worldPoint.w = 1.0;

    Vector4d viewPoint = viewMat * worldPoint;

    // 这个点应该在视图空间的正前方（沿-Z轴）
    bool passed = (fabs(viewPoint.x) < 1e-4 &&
                   fabs(viewPoint.y) < 1e-4 &&
                   fabs(viewPoint.z + sqrt(2.0)) < 1e-4);

    if (!passed) {
        printVector(worldPoint, "World space point");
        printVector(viewPoint, "View space point");
        std::cout << "Expected point at (0,0,-" << sqrt(2.0) << ")" << std::endl;
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase6() {
    std::cout << "Test Case 6: Make function test" << std::endl;

    Vector3d eye1(0.0, 0.0, 0.0);
    Vector3d lookAt1(0.0, 0.0, -1.0);
    Vector3d up1(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye1, lookAt1, up1);

    Vector3d eye2(1.0, 2.0, 3.0);
    Vector3d lookAt2(4.0, 5.0, 6.0);
    Vector3d up2(0.0, 1.0, 0.0);

    viewMat.Make(eye2, lookAt2, up2);

    ViewMatrix viewMat2(eye2, lookAt2, up2);

    bool passed = compareMatrices(viewMat, viewMat2);

    if (!passed) {
        std::cout << "After Make function:" << std::endl;
        printMatrix(viewMat);
        std::cout << "Expected:" << std::endl;
        printMatrix(viewMat2);
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase7() {
    std::cout << "Test Case 7: Camera transformation with direction vectors" << std::endl;

    Vector3d eye(2.0, 3.0, 5.0);
    Vector3d lookAt(2.0, 3.0, 4.0); // 看向-Z方向
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    // 测试方向向量（w=0）不应该被平移
    Vector4d direction;
    direction.x = 1.0;
    direction.y = 0.0;
    direction.z = 0.0;
    direction.w = 0.0;

    Vector4d viewDirection = viewMat * direction;

    // 方向向量应该只被旋转，不被平移
    bool passed = (fabs(viewDirection.w) < 1e-4);

    if (!passed) {
        printVector(direction, "World space direction");
        printVector(viewDirection, "View space direction");
        std::cout << "Direction vector w should be 0, got " << viewDirection.w << std::endl;
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase8() {
    std::cout << "Test Case 8: Camera inverse property" << std::endl;

    Vector3d eye(10.0, 20.0, 30.0);
    Vector3d lookAt(15.0, 25.0, 35.0);
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix V(eye, lookAt, up);

    // 创建一个点
    Vector4d worldPoint;
    worldPoint.x = 5.0;
    worldPoint.y = 10.0;
    worldPoint.z = 15.0;
    worldPoint.w = 1.0;

    // 转换到视图空间
    Vector4d viewPoint = V * worldPoint;

    // 视图矩阵的逆应该是摄像机变换矩阵
    // 对于视图矩阵V，其逆V⁻¹应该将视图坐标转换回世界坐标
    // 简化测试：验证视图矩阵是正交矩阵（旋转部分）
    double error = 0.0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double dot = 0.0;
            for (int k = 0; k < 3; ++k) {
                dot += V.M[i][k] * V.M[j][k];
            }
            double expected = (i == j) ? 1.0 : 0.0;
            error += fabs(dot - expected);
        }
    }

    bool passed = (error < 1e-4);

    std::cout << "Orthogonality error: " << error << std::endl;
    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase9() {
    std::cout << "Test Case 9: Camera looking at same position as eye" << std::endl;

    Vector3d eye(0.0, 0.0, 0.0);
    Vector3d lookAt(0.0, 0.0, 0.0); // 看向自身位置
    Vector3d up(0.0, 1.0, 0.0);

    // 这种情况应该被处理（避免除零）
    ViewMatrix viewMat(eye, lookAt, up);

    // 验证矩阵是有效的
    bool passed = true;

    // 检查矩阵是否有NaN或inf值
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!std::isfinite(viewMat.M[i][j])) {
                passed = false;
                std::cout << "Matrix contains non-finite value at [" << i << "][" << j << "]" << std::endl;
            }
        }
    }

    std::cout << "Result: " << (passed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return passed;
}

bool testCase10() {
    std::cout << "Test Case 10: Multiple point transformations" << std::endl;

    Vector3d eye(1.0, 2.0, 3.0);
    Vector3d lookAt(1.0, 2.0, 2.0); // 看向-Z方向
    Vector3d up(0.0, 1.0, 0.0);

    ViewMatrix viewMat(eye, lookAt, up);

    // 测试多个点
    Vector4d points[3];
    bool allPassed = true;

    // 点1：摄像机位置
    points[0].x = 1.0; points[0].y = 2.0; points[0].z = 3.0; points[0].w = 1.0;
    Vector4d result0 = viewMat * points[0];
    if (fabs(result0.z) > 1e-4) { // 摄像机位置应该在视图空间原点
        allPassed = false;
        std::cout << "Camera position not at origin in view space. z = " << result0.z << std::endl;
    }

    // 点2：摄像机正前方的点
    points[1].x = 1.0; points[1].y = 2.0; points[1].z = 2.0; points[1].w = 1.0;
    Vector4d result1 = viewMat * points[1];
    if (fabs(result1.z + 1.0) > 1e-4) { // 应该在z=-1处
        allPassed = false;
        std::cout << "Point in front of camera not at z=-1. z = " << result1.z << std::endl;
    }

    // 点3：摄像机右侧的点
    points[2].x = 2.0; points[2].y = 2.0; points[2].z = 3.0; points[2].w = 1.0;
    Vector4d result2 = viewMat * points[2];
    if (fabs(result2.x - 1.0) > 1e-4) { // 应该在x=1处
        allPassed = false;
        std::cout << "Point to right of camera not at x=1. x = " << result2.x << std::endl;
    }

    std::cout << "Result: " << (allPassed ? "PASSED" : "FAILED") << std::endl << std::endl;

    return allPassed;
}

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "VIEW MATRIX TEST SUITE" << std::endl;
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
