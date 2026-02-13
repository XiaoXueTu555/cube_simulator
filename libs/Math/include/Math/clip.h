//
// Created by xiaoxuetu on 2026/2/14.
//

#ifndef CUBE_SIMULATOR_CLIP_H
#define CUBE_SIMULATOR_CLIP_H
#include "vector4d.h"


namespace CS::Math
{

/* 计算顶点P1沿直线到P2，在平面 z + w = 0 上的交点 */
Vector4d ClipNearPlane(const Vector4d& p1, const Vector4d& p2);

}

#endif //CUBE_SIMULATOR_CLIP_H
