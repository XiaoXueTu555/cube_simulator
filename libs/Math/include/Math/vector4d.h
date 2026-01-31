//
// Created by xiaoxuetu on 2026/1/26.
// xiaoxuetu only designed the class interface in the header file, while YQM handled the development.
//

#ifndef MATH_VECTOR4D_H
#define MATH_VECTOR4D_H

#include <initializer_list>

#include "vector3d.h"

namespace CS::Math
{

/*
 * 四维向量，由浮点精度的 x、y、z、w 分量构成。
 */
class Vector4d
{
public:
    float x;
    float y;
    float z;
    float w;

    /* Vector3d 构造函数 */
    Vector4d() = default;
    Vector4d(float x, float y, float z, float w = 1);
    explicit Vector4d(std::initializer_list<float> list);

    /* 构造 Vector3d 的齐次化向量 */
    Vector4d(const Vector3d& InVector, float InW = 1);

    /* 点积 */
    float Dot(const Vector4d& V) const;
    static float DotProduct(const Vector4d& A, const Vector4d& B);

    /* 归一化 方向不变，变为一个单位向量 */
    void Normalize();

    /*
     * 设置向量的所有坐标。
     * @param InX 新的x坐标
     * @param InY 新的y坐标
     * @param InZ 新的z坐标
     * @param InW 新的w坐标
     */
    void Set(float InX, float InY, float InZ, float InW);

    /* 返回向量的长度 */
    float Length() const;

    /* 向量的加法 */
    Vector4d operator+(const Vector4d& V) const;
    Vector4d& operator+=(const Vector4d& V);

    /* 向量的减法 */
    Vector4d operator-(const Vector4d& V) const;
    Vector4d& operator-=(const Vector4d& V);

    /* 向量标量乘法 */
    Vector4d operator*(float Scale) const;
    Vector4d& operator*=(float Scale);

    /*
     * 每个坐标均除以Scale
     * @param Scale 你想对每个坐标除以的标量
     */
    Vector4d operator/(float Scale) const;
    Vector4d& operator/=(float Scale);

    bool Equal(const Vector4d& V, float Tolerance = CS_KINDA_SMALL_NUMBER) const;

    bool operator==(const Vector4d& V) const;
};

/* 向量标量乘法 */
Vector4d operator*(float Scale, const Vector4d& V);

}

#endif //MATH_VECTOR4D_H
