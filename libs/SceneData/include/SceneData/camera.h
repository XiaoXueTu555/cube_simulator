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
private:
    Math::Point eye_position{0, 0, 0};


private:
    Math::Point lookat_positon{0, 0, 1};
    Math::Vector3d up_direction = Math::Vector3d::UpVector;

    /* 近裁剪面距离、远裁剪面距离、视场角、宽高比 */
    float near = 1;
    float far = 600;
    float fov = CS_PI / 3; //弧度
    float aspect_ratio = 16.0f / 9.0f;

public:
    Camera();
    Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection = Math::Vector3d::UpVector);

public:
    float get_near()                         const { return near;           }
    float get_far()                          const { return far;            }
    float get_fov()                          const { return fov;            }
    float get_aspect_ratio()                 const { return aspect_ratio;   }
    const Math::Point& get_eye_position()    const { return eye_position;   }
    const Math::Point& get_lookat_positon()  const { return lookat_positon; }
    const Math::Vector3d& get_up_direction() const { return up_direction;   }

public:
    void set_near(float near);
    void set_far(float far);
    void set_fov(float fov);
    void set_aspect_ratio(float aspect_ratio);
    void SetEyePosition(const Math::Point& eye_position);
    void SetLookAtPosition(const Math::Point& lookat_position);
    void SetUpDirection(const Math::Point& up_direction);

    void SetLookDirection(const Math::Point& eye_position, const Math::Point& lookat_position);

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

private:
    bool BadLookDirection() const;
};

}


#endif //SCENEDATA_CAMERA_H
