//
// Created by xiaoxuetu on 2026/2/13.
//

#ifndef CUBE_SIMULATOR_CAMERA_ANIM_H
#define CUBE_SIMULATOR_CAMERA_ANIM_H
#include "Math/vector3d.h"

namespace CS
{
 /* 摄像机动画参数 */
struct CameraAnim
{
    bool is_playing = false;

    // 实际经过的时间
    float elapsed_time = 0.0f;

    //设定动画持续的时间
    float duration = 1.0f;

    CS::Math::Vector3d start_eye{0, 0, 0};
    CS::Math::Vector3d start_lookat{0, 0, -1};

    CS::Math::Vector3d end_eye{10, 0, 0};
    CS::Math::Vector3d end_lookat{0, 0, -1};
};
}

#endif //CUBE_SIMULATOR_CAMERA_ANIM_H
