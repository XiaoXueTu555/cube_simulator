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

Vector3d Lerp(const Vector3d& start_point, const Vector3d& end_point, float t)
{
    return Vector3d
    {
        Lerp(start_point.x, end_point.x, t),
        Lerp(start_point.y, end_point.y, t),
        Lerp(start_point.z, end_point.z, t)
    };
}

Vector3d CatmullRom(const Vector3d& p0, const Vector3d& p1, const Vector3d& p2,
    const Vector3d& p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    return 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
    );
}
}
