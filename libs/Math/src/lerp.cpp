//
// Created by xiaoxuetu on 2026/2/12.
//

#include "Math/lerp.h"

namespace CS::Math
{
static float Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Smoothstep(float t)
{
    return t * t * (3 - 2 * t);
}

Vector3d Lerp(const CS::Math::Vector3d& start_point, const CS::Math::Vector3d& end_point, float t)
{
    return CS::Math::Vector3d(
        Lerp(start_point.x, end_point.x, t),
        Lerp(start_point.y, end_point.y, t),
        Lerp(start_point.z, end_point.z, t)
    );
}
}
