//
// Created by xiaoxuetu on 2026/2/12.
//

#ifndef CUBE_SIMULATOR_LERP_H
#define CUBE_SIMULATOR_LERP_H
#include "vector3d.h"

namespace CS::Math
{

// 辅助函数：平滑插值 (让起步和停止更自然)
float Smoothstep(float t);

/*
 * 向量插值
 * t: 进度，百分比[0, 1]
 */
Vector3d Lerp(const Vector3d& start_point, const Vector3d& end_point, float t);

// 增加在 Animation.h 的 namespace CS 内部

// 4点 Catmull-Rom 插值函数
// p0: 前一个点, p1: 起点, p2: 终点, p3: 后一个点, t: 0.0~1.0
Vector3d CatmullRom(const Vector3d& p0, const Vector3d& p1, const Vector3d& p2,
    const Vector3d& p3, float t);

}

#endif //CUBE_SIMULATOR_LERP_H
