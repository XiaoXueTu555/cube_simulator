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

}

#endif //CUBE_SIMULATOR_LERP_H
