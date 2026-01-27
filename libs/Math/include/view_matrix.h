//
// Created by xiaoxuetu on 2026/1/27.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_VIEW_MATRIX_H
#define MATH_VIEW_MATRIX_H
#include "matrix.h"

namespace CS
{
namespace Math
{

/*
 * 视图矩阵
 * 需要三个量构建
 * EyePosition      摄像机位置
 * LookAtPosition   摄像机看向的位置
 * UpVector         世界正方向（右手坐标系）
 */
class ViewMatrix : public Matrix
{
public:
    using Matrix::M;

    /*
     * 构建视图矩阵
     * @param EyePosition      摄像机位置
     * @param LookAtPosition   摄像机看向的位置
     * @param UpVector         世界正方向（右手坐标系）
     */
    ViewMatrix(const Vector3d& EyePosition, const Vector3d& LookAtPosition, const Vector3d& UpVector);

    /*
     * 构建视图矩阵
     * @param EyePosition      摄像机位置
     * @param LookAtPosition   摄像机看向的位置
     * @param UpVector         世界正方向（右手坐标系）
     */
    void Make(const Vector3d& EyePosition, const Vector3d& LookAtPosition, const Vector3d& UpVector);
};
}
}

#endif //MATH_VIEW_MATRIX_H
