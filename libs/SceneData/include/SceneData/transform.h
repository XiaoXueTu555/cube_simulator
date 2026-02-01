//
// Created by xiaoxuetu on 2026/1/27.
//

#ifndef CUBE_SIMULATOR_TRANSFORM_H
#define CUBE_SIMULATOR_TRANSFORM_H

#include "Math/vector3d.h"

namespace CS::SceneData
{

/*
 * Transform
 * 用于表示物体的变换状态/姿态
 */
class Transform
{
public:
    Math::Vector3d Scale;
    Math::Vector3d Rotation;
    Math::Vector3d Position;

    /*
     * @param Scale 缩放量
     * @param Rotation 旋转量
     * @param Position 位置坐标
     */

    Transform();
    Transform(const Math::Vector3d& Scale, const Math::Vector3d& Rotation, const Math::Vector3d& Position);
};

}

#endif //CUBE_SIMULATOR_TRANSFORM_H
