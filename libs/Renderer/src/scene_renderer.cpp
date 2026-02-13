//
// Created by xiaoxuetu on 2026/1/31.
//

#include "Renderer/scene_renderer.h"
#include "Math/matrix.h"
#include "Math/scale_matrix.h"
#include "Math/rotation_matrix.h"
#include "Math/translation_matrix.h"
#include "Math/perspective_projection_matrix.h"
#include "Math/view_matrix.h"
#include "Math/clip.h"

#include <iostream>
#include <format>

namespace CS::Renderer
{

void SceneRenderer::Render(const SceneData::scene& game_scene, Viewport& view_port)
{
    /* 缩放矩阵 */
    static Math::ScaleMatrix mat_scale{};

    /* 旋转矩阵 */
    static Math::RotationMatrix mat_rotation{};

    /* 平移矩阵 */
    static Math::TranslationMatrix mat_translation{};

    /* 视图矩阵 */
    static Math::ViewMatrix mat_view;

    /* 透视矩阵 */
    static Math::PerspectiveProjectionMatrix mat_proj;

    /* 最终的合成矩阵 */
    static Math::Matrix mat_merge;

    /* 构建摄像机视图矩阵 */
    mat_view.Make(game_scene.camera.get_eye_position(),
        game_scene.camera.get_lookat_positon(),
        game_scene.camera.get_up_direction());

    /* 构建透视投影矩阵 */
    mat_proj.Make(game_scene.camera.get_fov(),
        game_scene.camera.get_near(),
        game_scene.camera.get_far(),
        game_scene.camera.get_aspect_ratio());

    /* 遍历所有物体 */
    for (const auto& game_object : game_scene.game_object_list)
    {
        /* 构建 game_object 的 Model 矩阵 */
        mat_scale.Make(game_object.transform.Scale);
        mat_rotation.Make(game_object.transform.Rotation);
        mat_translation.Make(game_object.transform.Position);

        // 合成所有的矩阵变换
        mat_merge = mat_proj * mat_view * mat_translation * mat_rotation * mat_scale;

        /* 遍历该物体的所有顶点 */
        for (int i = 0; i < game_object.mesh.Indices.size(); i += 3)
        {
            // 将顶点从模型坐标系变换到裁剪空间
            Math::Vector4d clip_space_point1 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 0]]};
            Math::Vector4d clip_space_point2 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 1]]};
            Math::Vector4d clip_space_point3 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 2]]};

            // ---------------------------------------------------------
            // 2. 裁剪逻辑 - 解决穿透、乱色、崩溃的核心
            // ---------------------------------------------------------
            // 判断一个点是否在摄像机前方 (OpenGL 风格: z + w >= 0)
            auto is_inside = [](const Math::Vector4d& v) {
                return v.z + v.w >= 0;
            };
            bool in1 = is_inside(clip_space_point1);
            bool in2 = is_inside(clip_space_point2);
            bool in3 = is_inside(clip_space_point3);
            // 统计有几个点在前面
            int in_count = (in1 ? 1 : 0) + (in2 ? 1 : 0) + (in3 ? 1 : 0);
            // 用来存放最终需要绘制的三角形列表
            // 一个原始三角形被裁剪后，可能变成 0个、1个 或 2个 新三角形
            std::vector<std::array<Math::Vector4d, 3>> triangles_to_draw;
            if (in_count == 0)
            {
                // 情况 A: 三个点都在摄像机后面 -> 直接丢弃，这就是防止“背后物体乱入”的关键
                continue;
            }
            else if (in_count == 3)
            {
                // 情况 B: 三个点都在前面 -> 完全安全，直接画
                triangles_to_draw.push_back({clip_space_point1, clip_space_point2, clip_space_point3});
            }
            else if (in_count == 1)
            {
                // 情况 C: 一个点在前面，两个在后面 -> 切割成一个小三角形
                // 找出那个幸存的点
                Math::Vector4d keep{};
                Math::Vector4d out1{}, out2{};

                if (in1) { keep = clip_space_point1; out1 = clip_space_point2; out2 = clip_space_point3; }
                else if (in2) { keep = clip_space_point2; out1 = clip_space_point3; out2 = clip_space_point1; }
                else { keep = clip_space_point3; out1 = clip_space_point1; out2 = clip_space_point2; }
                // 计算交点 (切边)
                Math::Vector4d new_p1 = ClipNearPlane(keep, out1);
                Math::Vector4d new_p2 = ClipNearPlane(keep, out2);
                // 形成新的小三角形
                triangles_to_draw.push_back({keep, new_p1, new_p2});
            }
            else if (in_count == 2)
            {
                // 情况 D: 两个点在前面，一个在后面 -> 切割成一个四边形，拆成两个三角形
                // 找出那个在后面的坏点
                Math::Vector4d out{};
                Math::Vector4d keep1{}, keep2{};

                if (!in1) { out = clip_space_point1; keep1 = clip_space_point2; keep2 = clip_space_point3; }
                else if (!in2) { out = clip_space_point2; keep1 = clip_space_point3; keep2 = clip_space_point1; }
                else { out = clip_space_point3; keep1 = clip_space_point1; keep2 = clip_space_point2; }
                // 计算交点 (切边)
                Math::Vector4d new_p1 = ClipNearPlane(keep1, out);
                Math::Vector4d new_p2 = ClipNearPlane(keep2, out);
                // 原三角形被切去一角，剩下四边形 keep1 -> new_p1 -> new_p2 -> keep2
                // 拆分为两个三角形绘制
                triangles_to_draw.push_back({keep1, new_p1, new_p2}); // 三角形 1
                triangles_to_draw.push_back({keep1, new_p2, keep2});  // 三角形 2
            }
            // ---------------------------------------------------------
            // 3. 光栅化绘制
            // ---------------------------------------------------------
            for (const auto& tri : triangles_to_draw)
            {
                // 传入裁剪后的坐标给 DrawTrangle
                view_port.DrawTrangle(
                    tri[0], tri[1], tri[2],
                    game_object.material_manager.GetChar(game_object.mesh.MaterialName[i / 3]),
                    game_object.material_manager.GetColor(game_object.mesh.MaterialName[i / 3])
                );
            }
        }
    }
}

}
