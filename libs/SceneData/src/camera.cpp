//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/camera.h"
#include "Math/vector4d.h"
#include "Math/rotation_matrix.h"

namespace CS::SceneData
{

Camera::Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection)
    :eye_position(EyePosition), lookat_positon(LookAtPosition), up_direction(UpDirection) {}

void Camera::SetEyePosition(const Math::Point& eye_positon)
{
    this->eye_position = eye_position;
}

void Camera::SetLookAtPosition(const Math::Point& lookat_positon)
{
    this->lookat_positon = lookat_positon;
}

void Camera::SetUpDirection(const Math::Point& up_direction)
{
    this->up_direction = up_direction;
}

void Camera::MoveCamera(float forward, float right, float up)
{
    /* 视角向量 */
    Math::Vector3d view_dir = this->lookat_positon - this->eye_position;
    view_dir.Normalize();

    /* 摄像机右侧方向向量 */
    Math::Vector3d right_dir = Math::Vector3d::CrossProduct(view_dir,this->up_direction);
    right_dir.Normalize();

    /* 计算移动偏移量 */
    Math::Vector3d move_offset  = (view_dir * forward) + (right_dir * right) + (up_direction * up);

    this->eye_position += move_offset;
    this->lookat_positon += move_offset;
}

void Camera::RotateView(float yaw, float pitch, bool use_radian)
{
    /* 获取当前的视线方向向量 */
    Math::Vector3d view_dir = lookat_positon - eye_position;
    Math::Vector4d view_dir4(view_dir.x, view_dir.y, view_dir.z, 1.0f);

    /* 构建旋转矩阵 */
    Math::RotationMatrix rotation(Math::Vector3d(pitch, yaw, 0.0f));

    /* 进行矩阵乘法 */
    Math::Vector4d new_dir4 = rotation * view_dir4;

    /*
     * 重新计算 LookAt 点 = 摄像机位置 + 旋转后的新方向
     * 为了防止多次浮点运算导致误差积累，对结果归一化
     */
    Math::Vector3d new_dir(new_dir4.x, new_dir4.y, new_dir4.z);
    new_dir.Normalize();
    lookat_positon = eye_position + new_dir;
}
}
