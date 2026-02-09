//
// Created by xiaoxuetu on 2026/2/6.
//

#include "Editor.h"
#include <format>
#include <cmath>
#include <fstream>

#include "Math/rotation_matrix.h"

namespace CS
{
Editor::Editor(ImGuiIO& io, SceneData::scene& scene, Renderer::SceneRenderer& renderer, Renderer::Viewport& port)
    : io(io), scene(scene), renderer(renderer), port(port)
{
    this->LoadSaveFile();
}

Editor::~Editor()
{
    this->SaveRecentlyOpenedFiles();
}

void Editor::ShowViewPortWindow()
{
    if (!this->show_view_port_window)
        return;

    this->MoveSceneCamera();

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

void Editor::ShowEditorWindow()
{
    this->GlobalShortcut();

    ImGui::Begin("Editor Window", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+Alt+O"))
            {
                this->show_add_game_object_window = true;
            }

            if (ImGui::BeginMenu("Open Recent"))
            {
                for (auto i = 0; i < 5; i++)
                {
                    if (!this->recently_opened_obj_files[i].empty())
                    {
                        if (ImGui::MenuItem(this->recently_opened_obj_files[i].c_str()))
                        {
                            this->AddGameObjectFormFiles(this->recently_opened_obj_files[i].c_str(),
                                                         this->recently_opened_yaml_files[i].c_str());
                        }
                    }
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Camera", "Ctrl+Alt+C"))
            {
                this->show_camera_window = !this->show_camera_window;
            }
            if (ImGui::MenuItem("GameObject", "Ctrl+Alt+G"))
            {
                this->show_game_object_window = !this->show_game_object_window;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::CollapsingHeader("Help"))
    {
        // 显示帮助文本
        static const char* help_text = R"(This is a terminal scene editor; imagine a virtual 3D space scene where a camera captures the view right before your eyes.
You can import a .obj model into the scene and use the Object Editor to adjust and view various model parameters, such as position, rotation, and scale.
You can also use the Camera Editor to change the viewing perspective; I hope this editor brings you some joy, so go ahead and use it!
)";
        ImGui::TextWrapped(help_text);
    }

    // 开启窗口选项
    if (ImGui::CollapsingHeader("Scene Show/Editor Windows"))
    {
        ImGui::Checkbox("Game View Port window", &this->show_view_port_window);
        ImGui::Checkbox("Camera Window", &this->show_camera_window);
        ImGui::Checkbox("GameObject Window", &this->show_game_object_window);
    }

    if (ImGui::CollapsingHeader("Render info"))
    {
        ImGui::SeparatorText("Scene Render info");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text(std::format("Number of objects in scene rendering = {}", this->obj_transforms.size()).c_str());

        // 计算场景中所有对象三角面数的总和
        size_t trangle_count = 0;
        for (const auto& obj : scene.game_object_list)
            trangle_count += (obj.mesh.Indices.size() / 3);
        ImGui::Text(std::format("Number of triangles in scene rendering = {}", trangle_count).c_str());

        ImGui::SeparatorText("ImGui Render info");
        ImGui::Text(std::format("ImGui Vertices Drawn Per Frame = {}", io.MetricsRenderVertices).c_str());
        ImGui::Text(std::format("ImGui Indices Drawn Per Frame = {}", io.MetricsRenderIndices).c_str());
    }
    else
    {
        ImGui::Text("Application FPS = %.1f", io.Framerate);
    }


    ImGui::End();

    /* 显示子窗口 */
    renderer.Render(scene, port);
    this->ShowViewPortWindow();
    port.Clear();

    this->ShowCameraWindow();
    this->ShowGameObjectWindow();
    this->ShowAddGameObjectWindow();

    if (this->show_imgui_debug_log_window)
        ImGui::ShowDebugLogWindow(&this->show_imgui_debug_log_window);
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
    ImGui::Text("");

    ImGui::SeparatorText("Camera Eye Position");
    static float drag_position_v_speed = 0.05;
    ImGui::DragFloat("Eye X", &this->camera_eye_position.x, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Eye Y", &this->camera_eye_position.y, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Eye Z", &this->camera_eye_position.z, drag_position_v_speed,-300,300);
    ImGui::Text("");

    ImGui::SeparatorText("Camera Look At Position");

    ImGui::DragFloat("Lookat X", &this->camera_lookat_position.x, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Lookat Y", &this->camera_lookat_position.y, drag_position_v_speed,-300,300);
    ImGui::DragFloat("Lookat Z", &this->camera_lookat_position.z, drag_position_v_speed,-300,300);

    ImGui::End();
}

void Editor::ShowGameObjectWindow()
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
            static float drag_scale_v_speed = 0.01;
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
            static float drag_position_v_speed = 0.01;
            ImGui::DragFloat("XPosition", &obj_transforms[item_selected_idx].Position.x, drag_position_v_speed,-300,300);
            ImGui::DragFloat("YPosition", &obj_transforms[item_selected_idx].Position.y, drag_position_v_speed,-300,300);
            ImGui::DragFloat("ZPosition", &obj_transforms[item_selected_idx].Position.z, drag_position_v_speed,-300,300);
            ImGui::TreePop();
        }

        ImGui::Text("");
        ImGui::SeparatorText("Other information");
        int vetrix_count = scene.game_object_list[item_selected_idx].mesh.Vertices.size();
        ImGui::Text(std::format("vertex count: {}", vetrix_count).c_str());

        int trianged_count = scene.game_object_list[item_selected_idx].mesh.Indices.size() / 3;
        ImGui::Text(std::format("triangle count: {}", trianged_count).c_str());
    }

    ImGui::End();
}

void Editor::ShowAddGameObjectWindow()
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
        this->AddGameObjectFormFiles(obj_filename, yaml_filename);

        // 更新最近打开文件列表
        for (auto i = 0; i < 4; i++)
        {
            this->recently_opened_obj_files[4 - i] = this->recently_opened_obj_files[3 - i];
            this->recently_opened_yaml_files[4 - i] = this->recently_opened_yaml_files[3 - i];
        }
        this->recently_opened_obj_files[0] = obj_filename;
        this->recently_opened_yaml_files[0] = yaml_filename;
    }
    ImGui::End();
}

void Editor::GlobalShortcut()
{
    bool Ctrl_Alt_Down = ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyDown(ImGuiMod_Alt);

    // ctrl + alt + o
    if (Ctrl_Alt_Down && ImGui::IsKeyPressed(ImGuiKey_O))
        this->show_add_game_object_window = true;

    // ctrl + alt + c
    if (Ctrl_Alt_Down && ImGui::IsKeyPressed(ImGuiKey_C))
        this->show_camera_window = true;

    // ctrl + alt + g
    if (Ctrl_Alt_Down && ImGui::IsKeyPressed(ImGuiKey_G))
        this->show_game_object_window = true;
}

void Editor::MoveSceneCamera()
{
    //鼠标右键
    bool mouse_right_key = ImGui::IsMouseDown(ImGuiMouseButton_Right);

    if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_W))
    {
        this->MoveCamera(this->camera_forward_speed * io.DeltaTime, 0, 0);
    }
    else if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_S))
    {
        this->MoveCamera(-this->camera_forward_speed * io.DeltaTime, 0, 0);
    }

    if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_A))
    {
        this->MoveCamera(0, -this->camera_right_speed * io.DeltaTime, 0);
    }
    else if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_D))
    {
        this->MoveCamera(0, this->camera_right_speed * io.DeltaTime, 0);
    }

    if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_Q))
    {
        this->MoveCamera(0, 0, this->camera_up_speed * io.DeltaTime);
    }
    else if (mouse_right_key && ImGui::IsKeyDown(ImGuiKey_E))
    {
        this->MoveCamera(0, 0, -this->camera_up_speed * io.DeltaTime);
    }

    // 鼠标控制
    if (mouse_right_key)
    {
        this->RotateView(-io.MouseDelta.x * this->camera_yaw_sensitivity,
                                -io.MouseDelta.y * this->camera_yaw_sensitivity, true);
    }
}

void Editor::AddGameObjectFormFiles(const char* obj_filename, const char* yaml_filename)
{
    try
    {
        scene.AddGameObject(obj_filename, yaml_filename);
        this->obj_transforms.push_back(scene.game_object_list.at(scene.game_object_list.size() - 1).transform);
        this->show_add_game_object_window = false;
    }
    catch (const std::exception& error)
    {
        this->show_imgui_debug_log_window = true;
        ImGui::DebugLog((error.what() + std::string{"\n"}).c_str());
    }
}

void Editor::SaveRecentlyOpenedFiles() const
{
    std::ofstream file;
    file.open("recently_file.txt");

    for (auto i = 0; i < 5; i++)
    {
        file << this->recently_opened_obj_files[i] << '\n';
        file << this->recently_opened_yaml_files[i] << '\n';
    }

    file.close();
}

void Editor::LoadSaveFile()
{
    std::ifstream file;
    file.open("recently_file.txt");

    // 可能是第一次运行而导致打不开文件，不做处理
    if (!file.is_open())  return;

    for (auto i = 0; i < 5; i++)
    {
        std::getline(file, this->recently_opened_obj_files[i]);
        std::getline(file, this->recently_opened_yaml_files[i]);
    }
    file.close();
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

const Math::Vector3d& Editor::get_camera_eye_position() const
{
    return this->camera_eye_position;
}

const Math::Vector3d& Editor::get_camera_lookat_position() const
{
    return this->camera_lookat_position;
}

const std::vector<SceneData::Transform>& Editor::get_obj_transforms() const
{
    return this->obj_transforms;
}

void Editor::MoveCamera(float forward, float right, float up)
{
    /* 视角向量 */
    Math::Vector3d view_dir = this->camera_lookat_position - this->camera_eye_position;
    view_dir.Normalize();

    /* 摄像机右侧方向向量 */
    Math::Vector3d right_dir = Math::Vector3d::CrossProduct(view_dir,this->camera_up_direction);
    right_dir.Normalize();

    /* 计算移动偏移量 */
    Math::Vector3d move_offset  = (view_dir * forward) + (right_dir * right) + (this->camera_up_direction * up);

    this->camera_eye_position += move_offset;
    this->camera_lookat_position += move_offset;
}

void Editor::RotateView(float yaw, float pitch, bool use_radian)
{
    // 1. 角度转弧度
    if (!use_radian)
    {
        yaw = yaw * CS_PI / 180.0f;
        pitch = pitch * CS_PI / 180.0f;
    }
    // 2. 获取当前视线方向向量并归一化
    // 注意：lookat_positon - eye_position 得到的是方向向量，不是位置点
    CS::Math::Vector3d view_dir = this->camera_lookat_position - this->camera_eye_position;
    view_dir.Normalize();
    // 3. 计算 Pitch (俯仰) —— 绕当前的“右向量”旋转
    // 公式：CrossProduct(Forward, Up) 在右手坐标系(Y向上)下得到的是 Right 向量
    CS::Math::Vector3d world_up = CS::Math::Vector3d::UpVector; // (0, 1, 0)
    CS::Math::Vector3d right_vec = CS::Math::Vector3d::CrossProduct(view_dir, world_up);

    // 防止视向量与 Y 轴平行（垂直向上或向下看）时，右向量为 0
    if (right_vec.Length() < CS_KINDA_SMALL_NUMBER)
    {
        right_vec = CS::Math::Vector3d::RightVector; // 默认使用 X 轴
    }
    else
    {
        right_vec.Normalize();
    }
    // 手动执行向量绕任意轴的旋转
    // 使用罗德里格旋转公式的简化版 (因为 view_dir 和 right_vec 是垂直的)
    // V_new = V * cos(theta) + (K x V) * sin(theta)
    // 这里轴 K 是 right_vec，theta 是 pitch
    CS::Math::Vector3d pitched_view = view_dir * std::cos(pitch) + CS::Math::Vector3d::CrossProduct(right_vec, view_dir) * std::sin(pitch);
    pitched_view.Normalize(); // 消除浮点误差
    // 4. 计算 Yaw (偏航) —— 绕世界坐标系的 Y 轴旋转
    // 使用你提供的 RotationMatrix 接口， Vector3d(0, yaw, 0) 对应绕 X, Y, Z 轴的分量
    // 我们只给 Y 轴分量赋值 yaw
    CS::Math::RotationMatrix yaw_rot_matrix(CS::Math::Vector3d(0.0f, yaw, 0.0f));
    // 将处理过 Pitch 的向量转为 Vector4d 进行矩阵乘法
    // 注意：w = 0，表示这是一个方向向量
    CS::Math::Vector4d pitched_dir4(pitched_view.x, pitched_view.y, pitched_view.z, 0.0f);
    CS::Math::Vector4d final_dir4 = yaw_rot_matrix * pitched_dir4;
    // 5. 更新 LookAt 位置
    CS::Math::Vector3d new_dir(final_dir4.x, final_dir4.y, final_dir4.z);
    new_dir.Normalize();
    this->camera_lookat_position = this->camera_eye_position + new_dir;
}
} // CS
