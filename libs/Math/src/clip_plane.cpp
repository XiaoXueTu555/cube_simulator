//
// Created by xiaoxuetu on 2026/2/5.
//

#include "Math/clip_plane.h"

namespace CS::Math
{
float ClipPlane::Distance(const Math::Vector4d& point) const
{
    return this->factor.Dot(point);
}

bool ClipPlane::IsInside(const Math::Vector4d& p, float Tolerance) const
{
    return this->Distance(p) <= Tolerance;
}

Math::Vector4d ClipPlane::Intersect(const Math::Vector4d& p1, const Math::Vector4d& p2)
{
    // 1. 计算 P1 和 P2 到平面的距离
    float d1 = this->Distance(p1);
    float d2 = this->Distance(p2);
    // 2. 计算插值系数 t
    // t = d1 / (d1 - d2)
    // 如果 d1 和 d2 符号相反，t 会在 0 到 1 之间
    float t = d1 / (d1 - d2);
    // 3. 线性插值 x, y, z, w
    // 公式： P_new = P1 + t * (P2 - P1)
    Math::Vector4d result;
    result.x = p1.x + t * (p2.x - p1.x);
    result.y = p1.y + t * (p2.y - p1.y);
    result.z = p1.z + t * (p2.z - p1.z);
    result.w = p1.w + t * (p2.w - p1.w);
    return result;
}
}
