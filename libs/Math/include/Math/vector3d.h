//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef CUBE_SIMULATOR_VECTOR3D_H
#define CUBE_SIMULATOR_VECTOR3D_H

#include <initializer_list>
#include "CSMathUtility.h"

namespace CS::Math
{

/*
 * 三维空间向量，由浮点精度的 x、y、z 分量构成。
 */

class Vector3d
{
public:
    float x;
    float y;
    float z;

    /*
     * 三维笛卡尔空间（右手坐标系）
            Y^
             |
             |
             |
            O+—————————> X
            /
          /
       Z/

     */

    /* (0, 0, 0) */
    static const Vector3d ZeroVector;

    /* (0, 1, 0) */
    static const Vector3d UpVector;

    /* (1, 0, 0) */
    static const Vector3d RightVector;

    /* (0, 0, -1) */
    static const Vector3d ForwardVector;

    /* (1, 0, 0) */
    static const Vector3d XAxisVector;

    /* (0, 1, 0) */
    static const Vector3d YAxisVector;

    /* (0, 0, 1) */
    static const Vector3d ZAxisVector;


    /* Vector3d 构造函数 */
    Vector3d() = default;
    Vector3d(float x, float y, float z);
    explicit Vector3d(std::initializer_list<float> list);

    /* 点积 */
    float Dot(const Vector3d& V) const;
    static float DotProduct(const Vector3d& A, const Vector3d& B);

    /* 向量积 */
    Vector3d Cross(const Vector3d& V) const;
    static Vector3d CrossProduct(const Vector3d& A, const Vector3d& B);

    /* 归一化 方向不变，变为一个单位向量 */
    void Normalize();

    /*
     * 设置向量的所有坐标。
     * @param InX 新的x坐标
     * @param InY 新的y坐标
     * @param InZ 新的z坐标
     */
    void Set(float InX, float InY, float InZ);

    /* 返回向量的长度 */
    float Length() const;

    /* 向量的加法 */
    Vector3d operator+(const Vector3d& V) const;
    Vector3d& operator+=(const Vector3d& V);

    /* 向量的减法 */
    Vector3d operator-(const Vector3d& V) const;
    Vector3d& operator-=(const Vector3d& V);

    /* 向量标量乘法 */
    Vector3d operator*(float Scale) const;
    Vector3d& operator*=(float Scale);

    /*
     * 每个坐标均除以Scale
     * @param Scale 你想对每个坐标除以的标量
     */
    Vector3d operator/(float Scale) const;
    Vector3d& operator/=(float Scale);

    bool Equal(const Vector3d& V, float Tolerance = CS_KINDA_SMALL_NUMBER) const;

    bool operator==(const Vector3d& V) const;
};

/* 向量标量乘法 */
Vector3d operator*(float Scale, const Vector3d& V);

using Point = Vector3d;

}

#endif //CUBE_SIMULATOR_VECTOR3D_H
