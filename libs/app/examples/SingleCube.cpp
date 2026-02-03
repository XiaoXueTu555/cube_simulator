//
// Created by xiaoxuetu on 2026/2/2.
//


#include <filesystem>
#include <iostream>

#include "SingleCube.h"

#include "SceneData/game_object.h"
#include "SceneData/scene.h"

#include "Renderer/view_port.h"
#include "Renderer/scene_renderer.h"

#include "imgui.h"

/* 数据准备 */

// 场景
static CS::SceneData::scene game_scene;

// 视口
static CS::Renderer::Viewport port{80, 45};

// 渲染器
static CS::Renderer::SceneRenderer renderer;

namespace CS::Examples
{

void SingleCubeInit()
{
    std::filesystem::path model_path = std::filesystem::current_path() / "../libs/app/examples/model";

    // 单个立方体
    static CS::SceneData::GameObject single_cube;
    if (!single_cube.LoadGamebjectFromFile(model_path/"cube.obj", model_path / "cube.yaml"))
    {
        throw std::invalid_argument("load file error: The file does not exist.");
    }
    single_cube.transform.Scale.Set(10, 10, 10);
    single_cube.transform.Position.Set(0, 0, -20);

    game_scene.AddGameObject(single_cube);

    /* 设定摄像机状态 */
    game_scene.camera.SetLookDirection(Math::Point{0, 0, 20}, Math::Point{0, 0, 0});
}

void SingleCubeRenderUI()
{
    renderer.Render(game_scene, port);
    /* 渲染视口 */
    ImGui::Begin("single cube view port");
    for (int i = 0; i < port.Height(); i++)
    {
        for (int j = 0; j < port.Width(); j++)
        {
            ImGui::TextColored(
                ImVec4
                {
                    port.color_buffer[i][j].x,
                    port.color_buffer[i][j].y,
                    port.color_buffer[i][j].z,
                    1.0f
                }, "%c", port.char_buffer[i][j]);
            if (j < port.Width() - 1)
                ImGui::SameLine();
        }
    }
    ImGui::End();
    port.Clear();

    ImGui::Begin("Editor");

    static const char* help_text = R"(
This is a 3D scene parameter editor.
The scene features a colorful cube with different colors on each face,
along with a camera. You can freely adjust the cube's position, rotation, and scale.
You can also modify the camera's position or viewing angle to observe the cube from different perspectives.
Have fun!"
)";
    ImGui::TextWrapped(help_text);

    ImGui::SeparatorText("Cube");

    ImGui::Text("Cube Transfrom");
    static float drag_position_v_speed = 0.05;
    static float drag_rotation_v_speed = 0.3;
    static float drag_scale_v_speed = 0.03;

    ImGui::Text("Position: ");
    ImGui::DragFloat("P X", &game_scene.game_object_list[0].transform.Position.x, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("P Y", &game_scene.game_object_list[0].transform.Position.y, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("P Z", &game_scene.game_object_list[0].transform.Position.z, drag_position_v_speed, -50.0f, 50.0f);

    ImGui::Text("Rotation: ");
    static float rx_angle = 0;
    static float ry_angle = 0;
    static float rz_angle = 0;
    ImGui::DragFloat("R X", &rx_angle, drag_rotation_v_speed, -360, 360);
    ImGui::DragFloat("R Y", &ry_angle, drag_rotation_v_speed, -360, 360);
    ImGui::DragFloat("R Z", &rz_angle, drag_rotation_v_speed, -360, 360);

    ImGui::Text("Scaling: ");
    ImGui::DragFloat("S X", &game_scene.game_object_list[0].transform.Scale.x, drag_scale_v_speed, 0.1f, 20.f);
    ImGui::DragFloat("S Y", &game_scene.game_object_list[0].transform.Scale.y, drag_scale_v_speed, 0.1f, 20.f);
    ImGui::DragFloat("S Z", &game_scene.game_object_list[0].transform.Scale.z, drag_scale_v_speed, 0.1f, 20.f);

    ImGui::SeparatorText("Camera");

    ImGui::Text("Cube Camera");
    static float fov_angle = 60;
    static float near = 2;
    static float far = 600;
    ImGui::DragFloat("FOV", &fov_angle, 0.05, 35, 70);
    ImGui::DragFloat("Near", &near, 0.001, 0.01f, 5.f);
    ImGui::DragFloat("Far", &far, 1, 0.1f, 1000.f);

    ImGui::Text("Camera eye position: ");
    static Math::Vector3d camera_eye_position{0, 0, 20};
    static Math::Vector3d camera_lookat_position{0, 0, 0};
    ImGui::DragFloat("Cam eye pos X", &camera_eye_position.x, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("Cam eye pos Y", &camera_eye_position.y, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("Cam eye pos Z", &camera_eye_position.z, drag_position_v_speed, -50.0f, 50.0f);

    ImGui::Text("Camera lookat position: ");
    ImGui::DragFloat("Cam lookat pos X", &camera_lookat_position.x, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("Cam lookat pos Y", &camera_lookat_position.y, drag_position_v_speed, -50.0f, 50.0f);
    ImGui::DragFloat("Cam lookat pos Z", &camera_lookat_position.z, drag_position_v_speed, -50.0f, 50.0f);

    ImGui::End();

    /* 赋值 */
    game_scene.game_object_list[0].transform.Rotation.Set(
        CS_AngleToRadian(rx_angle),
        CS_AngleToRadian(ry_angle),
        CS_AngleToRadian(rz_angle));

    game_scene.camera.set_fov(CS_AngleToRadian(fov_angle));
    game_scene.camera.set_near(near);
    game_scene.camera.set_far(far);

    game_scene.camera.SetEyePosition(camera_eye_position);
    game_scene.camera.SetLookAtPosition(camera_lookat_position);


}
}
