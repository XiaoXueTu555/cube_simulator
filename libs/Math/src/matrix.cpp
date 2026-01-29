//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#include "Math/matrix.h"

#include <cmath>
#include <stdexcept>

namespace CS
{
namespace Math
{
Matrix::Matrix()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

Matrix::Matrix(const Vector4d& InX, const Vector4d& InY, const Vector4d& InZ, const Vector4d& InW)
{
    M[0][0] = InX.x;
    M[1][0] = InX.y;
    M[2][0] = InX.z;
    M[3][0] = InX.w;
    M[0][1] = InY.x;
    M[1][1] = InY.y;
    M[2][1] = InY.z;
    M[3][1] = InY.w;
    M[0][2] = InZ.x;
    M[1][2] = InZ.y;
    M[2][2] = InZ.z;
    M[3][2] = InZ.w;
    M[0][3] = InW.x;
    M[1][3] = InW.y;
    M[2][3] = InW.z;
    M[3][3] = InW.w;
}

Matrix::Matrix(std::initializer_list<float> list)
{
    if (list.size() != 16)
        throw std::invalid_argument("Logic error:Matrix requires exactly 16 elements");

    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = list.begin()[index];
            index++;
        }
    }
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float> > list)
{
    int row = 0;
    for (auto& rowList: list)
    {
        if (rowList.size() != 4)
        {
            throw std::invalid_argument("Logic error:Each row must have exactly 4 elements");
        }

        int col = 0;
        for (float value: rowList)
        {
            M[row][col] = value;
            col++;
        }
        row++;
    }
}

Matrix Matrix::operator+(const Matrix& other)
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.M[i][j] = this->M[i][j] + other.M[i][j];
        }
    }
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->M[i][j] += other.M[i][j];
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix& other)
{
    /*类定义能够保证两矩阵一定能够相乘*/
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += this->M[i][k] * other.M[k][j];
            }
            result.M[i][j] = sum;
        }
    }
    return result;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    *this = *this * other;
    return *this;
}

Vector4d Matrix::operator*(const Vector4d& V)
{
    Vector4d result;
    result.x = M[0][0] * V.x + M[0][1] * V.y + M[0][2] * V.z + M[0][3] * V.w;
    result.y = M[1][0] * V.x + M[1][1] * V.y + M[1][2] * V.z + M[1][3] * V.w;
    result.z = M[2][0] * V.x + M[2][1] * V.y + M[2][2] * V.z + M[2][3] * V.w;
    result.w = M[3][0] * V.x + M[3][1] * V.y + M[3][2] * V.z + M[3][3] * V.w;
    return result;
}

Matrix Matrix::GetTransposed() const
{
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.M[i][j] = M[j][i];
        }
    }
    return result;
}

float Matrix::Determinant() const
{
    /*检查是否为仿射矩阵再计算性能更好*/
    return M[0][0] * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                      M[1][2] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) +
                      M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])) -
           M[0][1] * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                      M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                      M[1][3] * (M[2][0] * M[3][2] - M[2][2] * M[3][0])) +
           M[0][2] * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
                      M[1][1] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) +
                      M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0])) -
           M[0][3] * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
                      M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
                      M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
}

Matrix Matrix::Inverse() const
{
    if (std::fabs(Determinant()) < CS_KINDA_SMALL_NUMBER)
        throw std::invalid_argument("Logic error:Matrix is singular (non-invertible)");

    Matrix result;
    /*初等行变换求逆矩阵，待优化*/
    float temp[4][8];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[i][j] = M[i][j];
        }

        for (int j = 4; j < 8; j++)
        {
            temp[i][j] = (j == i + 4) ? 1.0f : 0.0f;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int pivot = i;
        for (int j = i + 1; j < 4; j++)
        {
            if (std::fabs(temp[j][i]) > std::fabs(temp[pivot][i]))
            {
                pivot = j;
            }
        }
        if (pivot != i)
        {
            for (int j = 0; j < 8; j++)
            {
                std::swap(temp[i][j], temp[pivot][j]);
            }
        }
        float factor = temp[i][i];
        for (int j = 0; j < 8; j++)
        {
            temp[i][j] /= factor;
        }
        for (int j = 0; j < 4; j++)
        {
            if (j != i)
            {
                float factor2 = temp[j][i];
                for (int k = 0; k < 8; k++)
                {
                    temp[j][k] -= factor2 * temp[i][k];
                }
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.M[i][j] = temp[i][j + 4];
        }
    }
    return result;
}
}
}
