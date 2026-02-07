//
// Created by xiaoxuetu on 2026/2/6.
//

#include "Editor.h"
#include <format>

namespace CS
{
void Editor::ShowViewPortWindow(const Renderer::Viewport& port, const ImGuiIO& io)
{
    if (!this->show_view_port_window)
        return;

    ImGui::Begin("Game View Port Window", &this->show_view_port_window, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PushFont(nullptr, this->port_font_size_base);

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

    ImGui::PopFont();
    ImGui::End();
}

void Editor::ShowEditorWindow(const ImGuiIO& io, Renderer::SceneRenderer& renderer, SceneData::scene& scene, Renderer::Viewport& port)
{
    ImGui::Begin("Editor Window", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                this->show_add_game_object_window = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Camera", "Ctrl+A"))
            {
                this->show_camera_window = !this->show_camera_window;
            }
            if (ImGui::MenuItem("GameObject", "Ctrl+B"))
            {
                this->show_game_object_window = !this->show_game_object_window;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // 显示帮助文本
    static std::string help_text = R"(hello world)";
    ImGui::Text(help_text.c_str());



    ImGui::SeparatorText("Render info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    // 开启窗口选项
    ImGui::Checkbox("Game View Port window", &this->show_view_port_window);
    ImGui::Checkbox("Camera Window", &this->show_camera_window);
    ImGui::Checkbox("GameObject Window", &this->show_game_object_window);

    ImGui::End();

    /* 显示子窗口 */
    renderer.Render(scene, port);
    this->ShowViewPortWindow(port, io);
    port.Clear();

    this->ShowCameraWindow();
    this->ShowGameObjectWindow(scene);
    this->ShowAddGameObjectWindow(scene);
}

void Editor::ShowCameraWindow()
{
    if (!this->show_camera_window)
        return;

    ImGui::Begin("Camera Window", &this->show_camera_window);
    ImGui::DragFloat("Camera Near",&camera_near,CS_KINDA_SMALL_NUMBER,5);
    ImGui::DragFloat("Camera Far",&camera_far,CS_KINDA_SMALL_NUMBER,1000);
    ImGui::DragFloat("Camera FOV",&camera_fov,CS_AngleToRadian(34.0f),CS_AngleToRadian(71.0f));
    ImGui::Text("Camera Aspect Ratio: 16.0f / 9.0f");

    ImGui::SeparatorText("Camera Eye Position");
    static float drag_position_v_speed = 0.05;
    ImGui::DragFloat("Eye X", &this->camera_eye_position.x, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Eye Y", &this->camera_eye_position.y, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Eye Z", &this->camera_eye_position.z, drag_position_v_speed,-300,300);

    ImGui::SeparatorText("Camera Look At Position");

    ImGui::DragFloat("Lookat X", &this->camera_lookat_position.x, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Lookat Y", &this->camera_lookat_position.y, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Lookat Z", &this->camera_lookat_position.z, drag_position_v_speed,-300,300);

    ImGui::End();
}

void Editor::ShowGameObjectWindow(SceneData::scene& scene)
{
    if (!this->show_game_object_window)
        return;

    ImGui::Begin("Game Object Window", &this->show_game_object_window);

    // 对象选择菜单
    static int item_selected_idx = -1;

    static std::string preview_str; //预览文字
    preview_str = "Please select an item.";

    if (item_selected_idx >= 0 && item_selected_idx < this->obj_transforms.size())
    {
        preview_str = "GameObject[" + std::to_string(item_selected_idx) + "]";
    }

    if (ImGui::BeginCombo("Game Object List", preview_str.c_str()))
    {
        for (int i = 0; i < this->obj_transforms.size(); i++)
        {
            const bool is_selected = (item_selected_idx == i);
            if (ImGui::Selectable(("GameObject[" + std::to_string(i) + "]").c_str(), is_selected))
                item_selected_idx = i;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (item_selected_idx >= 0)
    {
        ImGui::SeparatorText("Transform");
        if (ImGui::TreeNode("Scale"))
        {
            static float drag_scale_v_speed = 0.03;
            ImGui::DragFloat("XScale",&obj_transforms[item_selected_idx].Scale.x, drag_scale_v_speed, 0.01,100);
            ImGui::DragFloat("YScale",&obj_transforms[item_selected_idx].Scale.y, drag_scale_v_speed,0.01,100);
            ImGui::DragFloat("ZScale",&obj_transforms[item_selected_idx].Scale.z, drag_scale_v_speed,0.01,100);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Rotation"))
        {
            static float drag_rotation_v_speed = 0.3;
            ImGui::DragFloat("XRotation", &obj_transforms[item_selected_idx].Rotation.x, drag_rotation_v_speed, 0,
                             360, "%.1f", ImGuiSliderFlags_WrapAround);
            ImGui::DragFloat("YRotation", &obj_transforms[item_selected_idx].Rotation.y, drag_rotation_v_speed, 0,
                             360, "%.1f", ImGuiSliderFlags_WrapAround);
            ImGui::DragFloat("ZRotation", &obj_transforms[item_selected_idx].Rotation.z, drag_rotation_v_speed, 0,
                             360, "%.1f", ImGuiSliderFlags_WrapAround);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Position"))
        {
            static float drag_position_v_speed = 0.05;
            ImGui::DragFloat("XPosition", &obj_transforms[item_selected_idx].Position.x, drag_position_v_speed,-300,300);
            ImGui::DragFloat("YPosition", &obj_transforms[item_selected_idx].Position.y, drag_position_v_speed,-300,300);
            ImGui::DragFloat("ZPosition", &obj_transforms[item_selected_idx].Position.z, drag_position_v_speed,-300,300);
            ImGui::TreePop();
        }

        ImGui::Text("");
        ImGui::SeparatorText("Other information");
        if (ImGui::TreeNode("Vertice"))
        {
            int vetrix_count = scene.game_object_list[item_selected_idx].mesh.Vertices.size();
            ImGui::Text(std::format("vertex count: {}", vetrix_count).c_str());
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Triangle"))
        {
            int trianged_count = scene.game_object_list[item_selected_idx].mesh.Indices.size() / 3;
            ImGui::Text(std::format("triangle count: {}", trianged_count).c_str());
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void Editor::ShowAddGameObjectWindow(SceneData::scene& scene)
{
    if (!this->show_add_game_object_window)
        return;

    ImGui::Begin("Add a obj window", &this->show_add_game_object_window);
    static char obj_filename[256];
    static char yaml_filename[256];

    ImGui::InputText("path/filename.obj", obj_filename, sizeof(obj_filename));
    ImGui::InputText("path/filename.yaml", yaml_filename, sizeof(yaml_filename));
    if (ImGui::Button("load"))
    {
        try
        {
            scene.AddGameObject(obj_filename, yaml_filename);
            this->obj_transforms.push_back(scene.game_object_list.at(scene.game_object_list.size() - 1).transform);
            this->show_add_game_object_window = false;
        }
        catch (const std::exception& error)
        {
            ImGui::DebugLog(error.what());
        }
    }
    ImGui::End();
}

float Editor::get_camera_near() const
{
    return this->camera_near;
}

float Editor::get_camera_far() const
{
    return this->camera_far;
}

float Editor::get_camera_fov() const
{
    return this->camera_fov;
}

float Editor::get_camera_aspect_ratio() const
{
    return this->camera_aspect_ratio;
}

const std::vector<SceneData::Transform>& Editor::get_obj_transforms() const
{
    return this->obj_transforms;
}
} // CS
