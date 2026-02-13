//
// Created by xiaoxuetu on 2026/2/14.
//

#ifndef CUBE_SIMULATOR_ANIMATION_H
#define CUBE_SIMULATOR_ANIMATION_H

#include "Math/vector3d.h"
#include <vector>
#include <cmath>
#include "Math/lerp.h"
namespace CS
{
    // 动画播放模式
    enum class AnimPlayMode
    {
        Once,       // 播放一次，终点停止
        Loop,       // 循环播放 (A -> B -> A...)
        PingPong    // 往返播放 (A -> B -> A -> B...)
    };

    // 关键帧结构
    struct Vector3Keyframe
    {
        float time;     // 时间点 (秒)
        Math::Vector3d value; // 数值
        Vector3Keyframe(float t, Math::Vector3d v) : time(t), value(v) {}
    };

    // 动画轨道 (管理一系列关键帧)
    struct AnimTrack
    {
        std::vector<Vector3Keyframe> keys;

        // 添加关键帧 (不做复杂排序，假设按顺序添加)
        void AddKey(float time, const Math::Vector3d& value);

        // 核心采样函数：根据时间算值
        Math::Vector3d Sample(float time) const;

        float GetDuration() const;
    };

    // 完整的动画剪辑 (包含 P/R/S 三条轨道)
    struct AnimationClip
    {
        AnimTrack positionTrack;
        AnimTrack rotationTrack; // Euler Angles Radians
        AnimTrack scaleTrack;

        float duration = 1.0f;
        AnimPlayMode playMode = AnimPlayMode::Once;

        // 辅助函数：快速创建 A->B 的动画
        void SetupTwoKeys(const Math::Vector3d& start, const Math::Vector3d& end, float dur);
    };

    // 运行时状态
    struct AnimPlayerState
    {
        bool is_enabled = false;
        bool is_playing = false;
        float current_time = 0.0f; // 累积播放时间
        float speed = 1.0f;
    };

// ... 原有的 AnimationClip 等结构保持不变 ...

// 摄像机专用动画剪辑 (包含 Eye 和 LookAt 两条轨迹)
struct CameraAnimClip
{
    AnimTrack eyeTrack;      // 摄像机位置轨迹
    AnimTrack lookAtTrack;   // 视点轨迹

    float duration = 1.0f;
    AnimPlayMode playMode = AnimPlayMode::Once;

    // 辅助：快速获取总时长 (取两者最大值)
    float GetDuration() const;
};
}

#endif //CUBE_SIMULATOR_ANIMATION_H
