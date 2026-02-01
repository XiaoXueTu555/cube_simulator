//
// Created by xiaoxuetu on 2026/1/29.
//

#ifndef SCENEDATA_CAMERA_H
#define SCENEDATA_CAMERA_H
#include "Math/vector3d.h"
#include "Math/CSMathUtility.h"


namespace CS::SceneData
{

class Camera
{
public:
    Math::Point eye_position;
    Math::Point lookat_positon;
    Math::Vector3d up_direction = Math::Vector3d::UpVector;

    /* 近裁剪面距离、远裁剪面距离、视场角、宽高比 */
    float near = 0.3;
    float far = 1000;
    float fov = CS_PI / 3; //弧度
    float aspect_ratio = 16.0f / 9.0f;


    Camera() = default;
    Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection = Math::Vector3d::UpVector);

public:
    void SetEyePosition(const Math::Point& eye_positon);
    void SetLookAtPosition(const Math::Point& lookat_positon);
    void SetUpDirection(const Math::Point& up_direction);

    float& Near();
    float& Far();

public:
public:
    /*
     * 【移动】WASD 逻辑
     * forward: 前进距离 (+前进, -后退)
     * right:   右移距离 (+右, -左)
     * up:      上升距离 (+上, -下)
     */
    void MoveCamera(float forward, float right, float up);

    /*
     * 【旋转】鼠标视角逻辑
     * yaw:   水平旋转角度 (偏航)
     * pitch: 垂直旋转角度 (俯仰)
     */
    void RotateView(float yaw, float pitch, bool use_radian = false);
};

}


#endif //SCENEDATA_CAMERA_H
