//
// Created by xiaoxuetu on 2026/1/27.
//

#ifndef MATH_PROJECTION_MATRIX_H
#define MATH_PROJECTION_MATRIX_H

#include "matrix.h"

namespace CS::Math
{

/*
 * 透视矩阵
 */
class PerspectiveProjectionMatrix : public Matrix
{
public:
    using Matrix::M;

    using Matrix::Matrix;

    /*
     * 构建视图矩阵
     * @param FOV 视场角
     * @param Near : Near Clipping plane distance 近裁剪面距离
     * @param Far  : Far Clipping plane distance 远裁剪面距离
     */
    PerspectiveProjectionMatrix(float FOV, float Near, float Far,float AspectRation = 16.0f / 9.0f);

    /*
     * 构建视图矩阵
     * @param FOV 视场角
     * @param Near : Near Clipping plane distance 近裁剪面距离
     * @param Far  : Far Clipping plane distance 远裁剪面距离
     */
    void Make(float FOV, float Near, float Far,float AspectRation = 16.0f / 9.0f);
};

}

#endif //MATH_PROJECTION_MATRIX_H
