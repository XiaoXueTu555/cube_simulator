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

    this->fov = fov;
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

bool Camera::BadLookDirection() const
{
    if (std::abs((this->eye_position - this->lookat_positon).Length()) < CS_KINDA_SMALL_NUMBER)
    {
        return true;
    }
    return false;
}
}
