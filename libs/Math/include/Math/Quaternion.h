//
// Created by QuAn on 26-2-5.
//

#ifndef QUATERNION_H
#define QUATERNION_H
#include <initializer_list>

#include "matrix.h"
#include "vector3d.h"
#include "Math/rotator.h"

namespace CS::Math
{

/* 四元数类 q = a + bi + cj + dk (a,b,c,d ∈ R)*/
class Quaternion
{
public:
    float a;
    float b;
    float c;
    float d;

    Quaternion();//初始化为单位四元数

    Quaternion(float a, float b, float c, float d);
    explicit Quaternion(std::initializer_list<float> list);

    Quaternion operator+(const Quaternion& other) const;
    Quaternion& operator+=(const Quaternion& other);

    Quaternion operator-(const Quaternion& other) const;
    Quaternion& operator-=(const Quaternion& other);

    //四元数乘法
    Quaternion operator*(const Quaternion& other) const;
    Quaternion& operator*=(const Quaternion& other);

    //标量乘除
    Quaternion operator*(float scalar) const;
    Quaternion& operator*=(float scalar);
    Quaternion operator/(float scalar) const;
    Quaternion& operator/=(float scalar);

    void MakeIdentity();
    void Normalize();
    void Set(float a, float b, float c, float d);
    float Length() const;
    float Dot(const Quaternion& other)const;

    Quaternion FromAxisAngle(Vector3d& axis, float angle); //绕三维向量旋转angle弧度得到的四元数
    Quaternion FromEulerAngle(Rotator& V);//从欧拉角(弧度)得到四元数
    //四元数共轭
    Quaternion conjugate()const;
    //四元数的逆
    Quaternion Inverse()const;

    //线性插值
    Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t);
    //球面插值
    Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

};
}
#endif //QUATERNION_H
