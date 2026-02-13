//
// Created by xiaoxuetu on 2026/2/14.
//

#include "animation.h"

namespace CS
{
void AnimTrack::AddKey(float time, const Math::Vector3d& value)
{
    keys.emplace_back(time, value);
    // 简单冒泡排序保证时间递增
    for (size_t i = 0; i < keys.size() - 1; i++) {
        for (size_t j = i + 1; j < keys.size(); j++) {
            if (keys[j].time < keys[i].time) std::swap(keys[i], keys[j]);
        }
    }
}

// 编辑 Animation.h 中的 AnimTrack::Sample
Math::Vector3d AnimTrack::Sample(float time) const
{
    if (keys.empty()) return Math::Vector3d{0,0,0};
    if (keys.size() == 1) return keys[0].value;

    // 找到当前时间所在的区间
    for (size_t i = 0; i < keys.size() - 1; i++)
    {
        const auto& k1 = keys[i];
        const auto& k2 = keys[i+1];

        if (time >= k1.time && time < k2.time)
        {
            float duration = k2.time - k1.time;
            if (duration < 1e-5f) return k1.value;

            float t = (time - k1.time) / duration;
            float smooth_t = Math::Smoothstep(t); // 预先计算如果要用的话

            // 【核心逻辑：智能选择插值算法】

            // 只有2帧动画：保留 Smoothstep (缓入缓出)
            // 这种情况下，停顿反而是需要的（比如开关门）。
            if (keys.size() == 2)
            {
                return Lerp(k1.value, k2.value, smooth_t);
            }

            // 多帧动画：使用 Catmull-Rom 消除卡顿
            else
            {
                // 需要4个点：前(p0), 起(p1), 终(p2), 后(p3)
                // 获取前一个点
                Math::Vector3d p0 = (i == 0) ? k1.value : keys[i-1].value;
                // 获取后一个点
                Math::Vector3d p3 = (i + 2 >= keys.size()) ? k2.value : keys[i+2].value;

                // 使用 Catmull-Rom 计算
                return CatmullRom(p0, k1.value, k2.value, p3, t);
            }
        }
    }
    return keys.back().value;
}

float AnimTrack::GetDuration() const
{
    if (keys.empty()) return 0.0f;
    return keys.back().time;
}

void AnimationClip::SetupTwoKeys(const Math::Vector3d& start, const Math::Vector3d& end, float dur)
{
    positionTrack.keys.clear();
    positionTrack.AddKey(0.0f, start);
    positionTrack.AddKey(dur, end);
    duration = dur;
}

float CameraAnimClip::GetDuration() const
{
    float dur1 = eyeTrack.GetDuration();
    float dur2 = lookAtTrack.GetDuration();
    return std::max(dur1, dur2);
}
}
