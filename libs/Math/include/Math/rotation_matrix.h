//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_ROTATION_MATRIX_H
#define MATH_ROTATION_MATRIX_H

#include "matrix.h"

namespace CS
{
namespace Math
{

/* 旋转矩阵 */
class RotationMatrix : public Matrix
{
public:
    using Matrix::M;

    /*
     * 构造旋转矩阵
     * @param Rotation 三个轴上的旋转分量
     */
    RotationMatrix(const Vector3d& Rotation);

    /*
     * 构造旋转矩阵
     * @param Rotation 三个轴上的旋转分量
     */
    void Make(const Vector3d& Rotation);
};

}
}

#endif //MATH_ROTATION_MATRIX_H
