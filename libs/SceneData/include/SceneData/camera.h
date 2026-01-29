//
// Created by xiaoxuetu on 2026/1/29.
//

#ifndef SCENEDATA_CAMERA_H
#define SCENEDATA_CAMERA_H
#include "Math/vector3d.h"


namespace CS::SceneData
{

class Camera
{
public:
    Math::Point eye_position;
    Math::Point lookat_positon;
    Math::Vector3d up_direction;

    Camera() = default;

    Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection = Math::Vector3d::UpVector);
};

}


#endif //SCENEDATA_CAMERA_H
