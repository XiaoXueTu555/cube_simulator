//
// Created by QuAn on 26-2-5.
//

#ifndef ROTATOR_H
#define ROTATOR_H
#include "Quaternion.h"
#include "vector3d.h"

namespace CS::Math
{

/*
 * 欧拉角类
 * 偏航-俯仰-滚转 ZYX顺序
 */
class Rotator
{
public:
    float yaw; // yaw   绕z轴旋转
    float pitch; // pitch 绕y轴旋转
    float roll; // roll  绕x轴旋转

    /* 初始化为0度 */
    Rotator();
    Rotator(float InZAngle, float InYAngle, float InXAngle);
    Rotator(const Vector3d& V);
    explicit Rotator(std::initializer_list<float> list);

    void set(float InZAngle, float InYAngle, float InXAngle);
    void MakeIdentity();
};

}
#endif //ROTATOR_H
