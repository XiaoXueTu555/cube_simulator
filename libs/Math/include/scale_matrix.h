//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_SCALING_MATRIX_H
#define MATH_SCALING_MATRIX_H

#include "matrix.h"

namespace CS
{
namespace Math
{

/*
 * 缩放矩阵
 */
class ScaleMatrix : public Matrix
{
public:
    using Matrix::M;

    /*
     * 构造缩放矩阵
     * @param Scale 每个坐标分量上的缩放量
     */
    ScaleMatrix(const Vector3d& Scale);

    /*
     * 构造缩放矩阵
     * @param Scale 每个坐标分量上的缩放量
     */
    void Make(const Vector3d& Scale);
};

}
}

#endif //MATH_SCALING_MATRIX_H
