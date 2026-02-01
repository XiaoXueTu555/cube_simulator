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

void SceneRenderer::Renderer(const SceneData::scene& game_scene, Viewport& view_port)
{
    /* 缩放矩阵 */
    Math::ScaleMatrix mat_scale{};

    /* 旋转矩阵 */
    Math::RotationMatrix mat_rotation{};

    /* 平移矩阵 */
    Math::TranslationMatrix mat_translation{};

    /* 视图矩阵 */
    Math::ViewMatrix mat_view;

    /* 透视矩阵 */
    Math::PerspectiveProjectionMatrix mat_proj;

    /* 最终的合成矩阵 */
    static Math::Matrix mat_merge;

    /* 构建摄像机视图矩阵 */
    mat_view.Make(game_scene.camera.eye_position,
        game_scene.camera.lookat_positon,
        game_scene.camera.up_direction);

    /* 构建透视投影矩阵 */
    mat_proj.Make(game_scene.camera.fov,
        game_scene.camera.near,
        game_scene.camera.far,
        game_scene.camera.aspect_ratio);

    for (const auto& game_object : game_scene.game_object_list)
    {
        /* 构建 game_object 的 Model 矩阵 */
        mat_scale.Make(game_object.transform.Scale);
        mat_rotation.Make(game_object.transform.Rotation);
        mat_translation.Make(game_object.transform.Position);

        // 合成所有的矩阵变换
        mat_merge = mat_proj * mat_view * mat_translation * mat_rotation * mat_scale;

        // 将顶点从模型坐标系变换到裁剪空间
        for (int i = 0; i < game_object.mesh.Indices.size(); i += 3)
        {
            Math::Vector4d clip_space_point1 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 0]]};
            Math::Vector4d clip_space_point2 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 1]]};
            Math::Vector4d clip_space_point3 = mat_merge * Math::Vector4d{game_object.mesh.Vertices[game_object.mesh.Indices[i + 2]]};

            //透视除法
            clip_space_point1 /= clip_space_point1.w;
            clip_space_point2 /= clip_space_point2.w;
            clip_space_point3 /= clip_space_point3.w;

            /* NDC ---> 屏幕坐标系 */
            view_port.DrawTrangle(clip_space_point1, clip_space_point2, clip_space_point3,
                game_object.material_manager.GetChar (game_object.mesh.MaterialName[i / 3]), //该面对应的字符
                game_object.material_manager.GetColor(game_object.mesh.MaterialName[i / 3])); //该面对应的颜色
        }
    }
}

}
