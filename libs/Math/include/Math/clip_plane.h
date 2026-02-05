//
// Created by xiaoxuetu on 2026/2/5.
//

#ifndef APPLICATION_CLIP_PLANE_H
#define APPLICATION_CLIP_PLANE_H

#include "vector4d.h"

namespace CS::Math
{

/* 裁剪平面 */
class ClipPlane
{
public:
    /* 平面方程的系数 */
    Math::Vector4d factor;

    ClipPlane(const Math::Vector4d& factor);

    // 点到平面的有向距离
    float Distance(const Math::Vector4d& point) const;

    // 判断点是否在平面内部 (可视那一侧)
    // 假设 Distance <= 0 为内部（视锥体内），比如右平面 x-w=0，内部是 x<w，即 x-w < 0
    bool IsInside(const Math::Vector4d& p, float Tolerance = CS_KINDA_SMALL_NUMBER) const;

    // 求交点
    Math::Vector4d Intersect(const Math::Vector4d& p1, const Math::Vector4d& p2);
};

}

#endif //APPLICATION_CLIP_PLANE_H
