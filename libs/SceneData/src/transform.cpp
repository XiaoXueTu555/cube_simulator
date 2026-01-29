//
// Created by xiaoxuetu on 2026/1/27.
//

#include "SceneData/transform.h"

namespace CS::SceneData
{


Transform::Transform(const Math::Vector3d& Scale, const Math::Vector3d& Rotation, const Math::Vector3d& Position)
    : Scale(Scale), Rotation(Rotation), Position(Position) {}


}
