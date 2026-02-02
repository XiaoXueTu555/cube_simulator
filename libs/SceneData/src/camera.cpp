//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/camera.h"
#include "Math/vector4d.h"
#include "Math/rotation_matrix.h"
#include <cmath>
#include <exception>
#include <stdexcept>

namespace CS::SceneData
{
Camera::Camera()
{
    this->eye_position.Set(0, 0, 0);
    this->lookat_positon.Set(0, 0, 20);
}

Camera::Camera(Math::Point EyePosition, Math::Point LookAtPosition, Math::Vector3d UpDirection)
    :Camera() {}

void Camera::set_near(float near)
{
    if (near <= CS_KINDA_SMALL_NUMBER)
        throw std::logic_error("logic: camera.near so small");

    this->near = near;
}

void Camera::set_far(float far)
{
    if (far <= CS_KINDA_SMALL_NUMBER)
        throw std::logic_error("logic: camera.far so small");

    if (far > 1000)
        throw std::logic_error("logic: camera.far so big");

    this->far = far;
}

void Camera::set_fov(float fov)
{
    if (fov <= CS_AngleToRadian(34.0f) || fov >= CS_AngleToRadian(71.0f))
        throw std::invalid_argument("logic: camera.fov invalid");
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
    if (aspect_ratio <= CS_KINDA_SMALL_NUMBER)
        throw std::logic_error("logic: camera.aspect_ratio so small");

    this->aspect_ratio = aspect_ratio;
}

void Camera::SetEyePosition(const Math::Point& eye_position)
{
    this->eye_position = eye_position;

    if (BadLookDirection())
        throw std::logic_error("logic: camera look direction error");
}

void Camera::SetLookAtPosition(const Math::Point& lookat_position)
{
    this->lookat_positon = lookat_position;

    if (BadLookDirection())
        throw std::logic_error("logic: camera look direction error");
}

void Camera::SetUpDirection(const Math::Point& up_direction)
{
    this->up_direction = up_direction;
}

void Camera::SetLookDirection(const Math::Point& eye_position, const Math::Point& lookat_position)
{
    this->eye_position = eye_position;
    this->lookat_positon = lookat_position;

    if (BadLookDirection())
        throw std::logic_error("logic: camera look direction error");
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
    // 1. 角度转弧度
    if (!use_radian)
    {
        yaw = yaw * CS_PI / 180.0f;
        pitch = pitch * CS_PI / 180.0f;
    }
    // 2. 获取当前视线方向向量并归一化
    // 注意：lookat_positon - eye_position 得到的是方向向量，不是位置点
    CS::Math::Vector3d view_dir = lookat_positon - eye_position;
    view_dir.Normalize();
    // 3. 计算 Pitch (俯仰) —— 绕当前的“右向量”旋转
    // 公式：CrossProduct(Forward, Up) 在右手坐标系(Y向上)下得到的是 Right 向量
    CS::Math::Vector3d world_up = CS::Math::Vector3d::UpVector; // (0, 1, 0)
    CS::Math::Vector3d right_vec = CS::Math::Vector3d::CrossProduct(view_dir, world_up);

    // 防止视向量与 Y 轴平行（垂直向上或向下看）时，右向量为 0
    if (right_vec.Length() < CS_KINDA_SMALL_NUMBER)
    {
        right_vec = CS::Math::Vector3d::RightVector; // 默认使用 X 轴
    }
    else
    {
        right_vec.Normalize();
    }
    // 手动执行向量绕任意轴的旋转
    // 使用罗德里格旋转公式的简化版 (因为 view_dir 和 right_vec 是垂直的)
    // V_new = V * cos(theta) + (K x V) * sin(theta)
    // 这里轴 K 是 right_vec，theta 是 pitch
    CS::Math::Vector3d pitched_view = view_dir * std::cos(pitch) + CS::Math::Vector3d::CrossProduct(right_vec, view_dir) * std::sin(pitch);
    pitched_view.Normalize(); // 消除浮点误差
    // 4. 计算 Yaw (偏航) —— 绕世界坐标系的 Y 轴旋转
    // 使用你提供的 RotationMatrix 接口， Vector3d(0, yaw, 0) 对应绕 X, Y, Z 轴的分量
    // 我们只给 Y 轴分量赋值 yaw
    CS::Math::RotationMatrix yaw_rot_matrix(CS::Math::Vector3d(0.0f, yaw, 0.0f));
    // 将处理过 Pitch 的向量转为 Vector4d 进行矩阵乘法
    // 注意：w = 0，表示这是一个方向向量
    CS::Math::Vector4d pitched_dir4(pitched_view.x, pitched_view.y, pitched_view.z, 0.0f);
    CS::Math::Vector4d final_dir4 = yaw_rot_matrix * pitched_dir4;
    // 5. 更新 LookAt 位置
    CS::Math::Vector3d new_dir(final_dir4.x, final_dir4.y, final_dir4.z);
    new_dir.Normalize();
    lookat_positon = eye_position + new_dir;
}

bool Camera::BadLookDirection() const
{
    if (std::abs((this->eye_position - this->lookat_positon).Length()) < CS_KINDA_SMALL_NUMBER)
    {
        return true;
    }
    return false;
}
}
