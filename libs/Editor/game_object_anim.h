//
// Created by xiaoxuetu on 2026/2/13.
//

#ifndef CUBE_SIMULATOR_GAME_OBJECT_ANIM_H
#define CUBE_SIMULATOR_GAME_OBJECT_ANIM_H
#include "SceneData/transform.h"

namespace CS
{
/* 游戏对象的动画参数 */
struct GameObjectAnim
{
    bool is_enabled = false;   // 是否启用该对象的动画（总开关）
    bool is_playing = false;   // 是否正在播放

    float elapsed_time = 0.0f; // 已播放时间
    float duration = 2.0f;     // 动画持续时间
    // 起始变换
    SceneData::Transform start_transform;

    // 目标变换
    SceneData::Transform target_transform;
};
}

#endif //CUBE_SIMULATOR_GAME_OBJECT_ANIM_H
