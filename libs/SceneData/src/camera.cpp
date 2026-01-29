//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/camera.h"

namespace CS::SceneData
{

Camera::Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection)
    :eye_position(EyePosition), lookat_positon(LookAtPosition), up_direction(UpDirection) {}

}
