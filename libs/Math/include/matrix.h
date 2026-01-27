//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H
#include "vector4d.h"

namespace CS
{
namespace Math
{

/*
 * 4x4 Matrix
 * 要注意！矩阵乘法应用的方式使得 C = A * B 会生成一个逻辑上先应用 A 再应用 B 的变换 C。
 */
class Matrix
{
public:
    /* 4x4 个矩阵元素 */
    float M[4][4];

    /* 初始化为单位矩阵 */
    Matrix();
    Matrix(const Vector4d& InX, const Vector4d& InY, const Vector4d& InZ, const Vector4d& InW);
    Matrix(std::initializer_list<float> list);
    Matrix(std::initializer_list<std::initializer_list<float>> list);

    Matrix operator+(const Matrix& other);
    Matrix& operator+=(const Matrix& other);

    Matrix operator*(const Matrix& other);
    Matrix& operator*=(const Matrix& other);

    Vector4d operator*(const Vector4d& V);

    /* 转置矩阵 */
    Matrix GetTransposed() const;

    /* 行列式 */
    float Determinant() const;

    /* 求逆矩阵 */
    Matrix Inverse() const;
};

}
}

#endif //MATH_MATRIX_H
