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
    ImGui::Text("Cube Transfrom");

    ImGui::Text("Position: ");
    ImGui::SliderFloat("P X", &game_scene.game_object_list[0].transform.Position.x, -50.0f, 50.0f);
    ImGui::SliderFloat("P Y", &game_scene.game_object_list[0].transform.Position.y, -50.0f, 50.0f);
    ImGui::SliderFloat("P Z", &game_scene.game_object_list[0].transform.Position.z, -50.0f, 50.0f);

    ImGui::Text("Rotation: ");
    ImGui::SliderAngle("R X", &game_scene.game_object_list[0].transform.Rotation.x);
    ImGui::SliderAngle("R Y", &game_scene.game_object_list[0].transform.Rotation.y);
    ImGui::SliderAngle("R Z", &game_scene.game_object_list[0].transform.Rotation.z);

    ImGui::Text("Scaling: ");
    ImGui::SliderFloat("S X", &game_scene.game_object_list[0].transform.Scale.x, 0.1f, 20.f);
    ImGui::SliderFloat("S Y", &game_scene.game_object_list[0].transform.Scale.y, 0.1f, 20.f);
    ImGui::SliderFloat("S Z", &game_scene.game_object_list[0].transform.Scale.z, 0.1f, 20.f);

    ImGui::Text("Cube Camera");
    static float fov = CS_PI / 3;
    static float near = 0.1;
    static float far = 100;
    ImGui::SliderAngle("FOV", &fov, 35, 70);
    ImGui::SliderFloat("Near", &near, 0.01f, 5.f);
    ImGui::SliderFloat("Far", &far, 0.1f, 200.f);

    ImGui::Text("Camera eye position: ");
    Math::Vector3d camera_eye_position{0, 0, 20};
    Math::Vector3d camera_lookat_position{0, 0, 0};
    ImGui::SliderFloat("Cam eye pos X", &camera_eye_position.x, -50.0f, 50.0f);
    ImGui::SliderFloat("Cam eye pos Y", &camera_eye_position.y, -50.0f, 50.0f);
    ImGui::SliderFloat("Cam eye pos Z", &camera_eye_position.z, -50.0f, 50.0f);

    ImGui::Text("Camera lookat position: ");
    ImGui::SliderFloat("Cam lookat pos X", &camera_lookat_position.x, -50.0f, 50.0f);
    ImGui::SliderFloat("Cam lookat pos Y", &camera_lookat_position.y, -50.0f, 50.0f);
    ImGui::SliderFloat("Cam lookat pos Z", &camera_lookat_position.z, -50.0f, 50.0f);

    ImGui::End();

    game_scene.camera.set_fov(fov);
    game_scene.camera.set_near(near);
    game_scene.camera.set_far(far);

    game_scene.camera.SetEyePosition(camera_eye_position);
    game_scene.camera.SetLookAtPosition(camera_lookat_position);
}
}
