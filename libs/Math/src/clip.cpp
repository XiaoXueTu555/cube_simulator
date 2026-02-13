//
// Created by xiaoxuetu on 2026/2/14.
//

#include "Math/clip.h"

#include <cmath>

namespace CS::Math
{
Vector4d ClipNearPlane(const Vector4d& p1, const Vector4d& p2)
{
    // 平面方程: z + w = 0
    // 计算 t 值 (插值参数)
    // 在 p1 处的值: d1 = p1.z + p1.w
    // 在 p2 处的值: d2 = p2.z + p2.w
    // 我们想找 d = 0 的点
    // t = d1 / (d1 - d2)

    float d1 = p1.z + p1.w;
    float d2 = p2.z + p2.w;

    // 防止除以0 (平行的情况)
    if (std::fabs(d1 - d2) < 1e-6f) return p1;
    float t = d1 / (d1 - d2);
    // 线性插值
    return Math::Vector4d
    {
        p1.x + t * (p2.x - p1.x),
        p1.y + t * (p2.y - p1.y),
        p1.z + t * (p2.z - p1.z),
        p1.w + t * (p2.w - p1.w)
    };
}
}
