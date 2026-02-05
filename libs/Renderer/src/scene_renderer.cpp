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

            /* Sutherland-Hodgman Clipping algorithm (待续...) */
            auto is_outside = [](const Math::Vector4d& v) {
                // 这里假设你的投影矩阵把 z 映射到了 0 到 w 之间
                // 如果是 OpenGL 风格，范围是 -w 到 w
                return v.z < 0 || v.z > v.w;
            };
            //剔除近裁剪面以及远裁剪面之外的三角形
            if (is_outside(clip_space_point1) &&
                is_outside(clip_space_point2) &&
                is_outside(clip_space_point3))
                            continue;

            /* NDC ---> 屏幕坐标系 */
            view_port.DrawTrangle(clip_space_point1, clip_space_point2, clip_space_point3,
                game_object.material_manager.GetChar (game_object.mesh.MaterialName[i / 3]), //该面对应的字符
                game_object.material_manager.GetColor(game_object.mesh.MaterialName[i / 3])); //该面对应的颜色
        }
    }
}

}
