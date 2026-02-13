//
// Created by xiaoxuetu on 2026/2/6.
//

#include "Editor.h"
#include "Math/lerp.h"
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
    // --- 新增：初始化摄像机动画默认值，防止空指针或零向量导致崩溃 ---
    // 默认摄像机位置 (0, 0, 10) 看向 (0, 0, 0)
    Math::Vector3d default_eye(0, 0, 10);
    Math::Vector3d default_lookat(0, 0, 0);
    // 添加第一帧
    camera_anim_clip.eyeTrack.AddKey(0.0f, default_eye);
    camera_anim_clip.lookAtTrack.AddKey(0.0f, default_lookat);
    // 同步更新当前摄像机位置 (防止开局闪瞬移)
    this->camera_eye_position = default_eye;
    this->camera_lookat_position = default_lookat;
}

Editor::~Editor()
{
    this->SaveRecentlyOpenedFiles();
}

void Editor::ShowViewPortWindow()
{
    if (!this->show_view_port_window)
        return;

    //移动摄像机
    if (!this->camera_anim_state.is_playing)
    {
        this->MoveSceneCamera();
    }

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
    this->GlobalShortcut(); //全局快捷键
    this->UpdateAnimationSystem();

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

        if (ImGui::BeginMenu("Other"))
        {
            if (ImGui::MenuItem("Exit the program", "Ctrl+Alt+Q"))
            {
                this->exit_the_program = true;
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
        ImGui::Text("This is a free software! Link:");
        ImGui::TextLinkOpenURL("https://github.com/XiaoXueTu555/cube_simulator");
        ImGui::TextWrapped(help_text);
        ImGui::TextColored(ImVec4{1, 0, 0, 1},
            "Just a heads-up: right-clicking activates camera\n control mode,"
            " so your mouse movements will change\n the viewing angle!"
            " You can also use WASDQE to move the camera\n around—this is probably the most useful feature!");
    }

    // 开启窗口选项
    if (ImGui::CollapsingHeader("Scene Show/Editor Windows"))
    {
        ImGui::Checkbox("Game View Port window", &this->show_view_port_window);
        ImGui::Checkbox("Camera Window", &this->show_camera_window);
        ImGui::Checkbox("GameObject Window", &this->show_game_object_window);

        ImGui::SeparatorText("anim");

        ImGui::Checkbox("Camera Anim Window", &this->show_camera_anim_window);
        ImGui::Checkbox("Anim Editor", &this->show_animation_editor_window);

        ImGui::SeparatorText("Anim Control");
        // 这里的逻辑非常清晰：点的播放就传true，点停止就传false
        if (ImGui::Button("Play All Anims"))
        {
            this->PlayAllAnimations(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop All Anims"))
        {
            this->PlayAllAnimations(false);
        }
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
    this->ShowCameraAnimWindow();
    this->ShowAnimationEditorWindow();

    if (this->show_imgui_debug_log_window)
        ImGui::ShowDebugLogWindow(&this->show_imgui_debug_log_window);
}

void Editor::ShowCameraWindow()
{
    if (!this->show_camera_window)
        return;

    ImGui::Begin("Camera Window", &this->show_camera_window);
    ImGui::DragFloat("Camera Near",&camera_near,0.001, CS_KINDA_SMALL_NUMBER * 10,5, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat("Camera Far",&camera_far,0.01,CS_KINDA_SMALL_NUMBER * 10,1000, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat("Camera FOV",&camera_fov,0.01,CS_AngleToRadian(34.0f),CS_AngleToRadian(71.0f), "%.3f", ImGuiSliderFlags_AlwaysClamp);
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

    if (ImGui::TreeNode("Move/Around Speed"))
    {
        ImGui::DragFloat("Camera forward", &this->camera_forward_speed, 0.001,0.01, 10);
        ImGui::DragFloat("Camera right", &this->camera_right_speed, 0.001,0.01,10);
        ImGui::DragFloat("Camera up", &this->camera_up_speed, 0.001, 0.01, 10);
        ImGui::DragFloat("Camera yaw sensitivity", &this->camera_yaw_sensitivity, 0.0001, 0.0001, 0.1);
        ImGui::DragFloat("Camera pitch sensitivity", &this->camera_pitch_sensitivity, 0.0001, 0.0001, 0.1);
        ImGui::TreePop();
    }

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
            auto& scale = this->obj_transforms[item_selected_idx].Scale;
            static float drag_scale_v_speed = 0.01; //滑动速率

            if (float uniform_scale = scale.x;
                ImGui::DragFloat("Uniform Scale", &uniform_scale, drag_scale_v_speed, 0.01, 20))
            {
                // 当这个滑块变动时，等比例缩放
                scale.x = scale.y = scale.z = uniform_scale;
            }
            ImGui::Separator();

            ImGui::DragFloat("XScale",&scale.x, drag_scale_v_speed, 0.01,20);
            ImGui::DragFloat("YScale",&scale.y, drag_scale_v_speed,0.01,20);
            ImGui::DragFloat("ZScale",&scale.z, drag_scale_v_speed,0.01,20);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Rotation"))
        {
            auto& rotation = this->obj_transforms[item_selected_idx].Rotation;
            static float drag_rotation_v_speed = 0.3; //滑动速率

            ImGui::DragFloat("XRotation", &rotation.x, drag_rotation_v_speed, 0, 360, "%.1f",
                             ImGuiSliderFlags_WrapAround);
            ImGui::DragFloat("YRotation", &rotation.y, drag_rotation_v_speed, 0, 360, "%.1f",
                             ImGuiSliderFlags_WrapAround);
            ImGui::DragFloat("ZRotation", &rotation.z, drag_rotation_v_speed, 0, 360, "%.1f",
                             ImGuiSliderFlags_WrapAround);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Position"))
        {
            auto& position = this->obj_transforms[item_selected_idx].Position;
            static float drag_position_v_speed = 0.01; //滑动速率

            ImGui::DragFloat("XPosition", &position.x, drag_position_v_speed,-300,300);
            ImGui::DragFloat("YPosition", &position.y, drag_position_v_speed,-300,300);
            ImGui::DragFloat("ZPosition", &position.z, drag_position_v_speed,-300,300);
            ImGui::TreePop();
        }

        ImGui::Text("");
        ImGui::SeparatorText("Other information");

        int vetrix_count = scene.game_object_list[item_selected_idx].mesh.Vertices.size();
        ImGui::Text(std::format("vertex count: {}", vetrix_count).c_str());

        int trianged_count = scene.game_object_list[item_selected_idx].mesh.Indices.size() / 3;
        ImGui::Text(std::format("triangle count: {}", trianged_count).c_str());

        //从场景中删除这个对象
        if (ImGui::Button("DELETE"))
        {
            const auto& obj_it = this->scene.game_object_list.begin() + item_selected_idx;
            this->scene.game_object_list.erase(obj_it);

            const auto& obj_transform_it = this->obj_transforms.begin() + item_selected_idx;
            this->obj_transforms.erase(obj_transform_it);

            if (item_selected_idx >= 0 && item_selected_idx < obj_anim_clips.size())
            {
                obj_anim_clips.erase(obj_anim_clips.begin() + item_selected_idx);
                obj_anim_states.erase(obj_anim_states.begin() + item_selected_idx);
            }

            item_selected_idx = -1;
        }
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

    // ctrl + alt + q
    if (Ctrl_Alt_Down && ImGui::IsKeyPressed(ImGuiKey_Q))
        this->exit_the_program = true;
}

void Editor::ShowCameraAnimWindow()
{
    if (!this->show_camera_anim_window) return;

    ImGui::Begin("Camera Animation", &this->show_camera_anim_window);

    auto& clip = this->camera_anim_clip;
    auto& state = this->camera_anim_state;

    // --- 标题栏控制 ---
    ImGui::Checkbox("Enable", &state.is_enabled);
    ImGui::SameLine();
    if (ImGui::Button(state.is_playing ? "Pause" : "Play"))
    {
        state.is_playing = !state.is_playing;
        if (state.is_playing)
        {
            state.current_time = 0.0f;
            state.is_enabled = true;
        }
    }

    ImGui::Separator();

    // --- 更新总时长 ---
    if (ImGui::DragFloat("Total Duration", &clip.duration, 0.1f, 0.1f, 60.0f, "%.1fs"))
    {
        if (clip.eyeTrack.keys.size() >= 2) clip.eyeTrack.keys.back().time = clip.duration;
        if (clip.lookAtTrack.keys.size() >= 2) clip.lookAtTrack.keys.back().time = clip.duration;
    }

    // --- 预览时间轴 (关键修改：只改时间，不动摄像机) ---
    if (!state.is_playing)
    {
        // 【修改】这里只管改时间，不对摄像机进行采样！
        // 这样你拖动进度条的时候，摄像机就不再乱跳了
        ImGui::DragFloat("Record Time (s)", &state.current_time, 0.01f, 0.0f, clip.duration, "%.2f");

        // 加个提示，告诉用户怎么用
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Set time, then move camera, then Add Key.");
    }
    else
    {
        // 播放时的进度条逻辑 (保持不变)
        float display_progress = 0.0f;
        if (clip.playMode == AnimPlayMode::Once)
        {
            display_progress = state.current_time / clip.duration;
            if (display_progress > 1.0f) display_progress = 1.0f;
        }
        else if (clip.playMode == AnimPlayMode::Loop)
        {
            display_progress = fmod(state.current_time, clip.duration) / clip.duration;
        }
        else if (clip.playMode == AnimPlayMode::PingPong)
        {
            float cycle = fmod(state.current_time, clip.duration * 2.0f);
            if (cycle <= clip.duration) display_progress = cycle / clip.duration;
            else display_progress = 1.0f - (cycle - clip.duration) / clip.duration;
        }
        ImGui::Text("Playing... %.2fs / %.2fs", state.current_time, clip.duration);
        ImGui::ProgressBar(display_progress, ImVec2(0, 0));
    }

    ImGui::SeparatorText("Keyframes");

    int mode = static_cast<int>(clip.playMode);
    const char* modes[] = { "Once", "Loop", "PingPong" };
    if (ImGui::Combo("Mode", &mode, modes, 3))
    {
        clip.playMode = static_cast<AnimPlayMode>(mode);
    }

    ImGui::Spacing();

    // --- 1. Eye Position Track ---
    if (ImGui::CollapsingHeader("Eye Position Track", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Add Key##Eye"))
        {
            // 添加关键帧时，才真正把当前摄像机位置记录下来
            clip.eyeTrack.AddKey(state.current_time, this->camera_eye_position);
            if (state.current_time > clip.duration) clip.duration = state.current_time;
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##Eye")) clip.eyeTrack.keys.clear();

        ImGui::Indent();
        for (size_t k = 0; k < clip.eyeTrack.keys.size(); k++)
        {
            auto& key = clip.eyeTrack.keys[k];
            ImGui::PushID(k);
            ImGui::SetNextItemWidth(60);
            if (ImGui::DragFloat("##T", &key.time, 0.01f, 0.0f, 60.0f, "T: %.2f"))
            {
                if (key.time > clip.duration) clip.duration = key.time;
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(180);
            ImGui::DragFloat3("##Pos", &key.value.x, 0.1f);
            ImGui::SameLine();
            if (clip.eyeTrack.keys.size() > 1)
            {
                if (ImGui::SmallButton("Del"))
                {
                    clip.eyeTrack.keys.erase(clip.eyeTrack.keys.begin() + k);
                    k--;
                }
            }
            ImGui::PopID();
        }
        ImGui::Unindent();
        ImGui::Spacing();
    }

    // --- 2. LookAt Position Track ---
    if (ImGui::CollapsingHeader("LookAt Position Track", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Add Key##LookAt"))
        {
            clip.lookAtTrack.AddKey(state.current_time, this->camera_lookat_position);
            if (state.current_time > clip.duration) clip.duration = state.current_time;
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##LookAt")) clip.lookAtTrack.keys.clear();

        ImGui::Indent();
        for (size_t k = 0; k < clip.lookAtTrack.keys.size(); k++)
        {
            auto& key = clip.lookAtTrack.keys[k];
            ImGui::PushID(k + 1000);
            ImGui::SetNextItemWidth(60);
            if (ImGui::DragFloat("##T", &key.time, 0.01f, 0.0f, 60.0f, "T: %.2f"))
            {
                if (key.time > clip.duration) clip.duration = key.time;
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(180);
            ImGui::DragFloat3("##Pos", &key.value.x, 0.1f);
            ImGui::SameLine();
            if (clip.lookAtTrack.keys.size() > 1)
            {
                if (ImGui::SmallButton("Del"))
                {
                    clip.lookAtTrack.keys.erase(clip.lookAtTrack.keys.begin() + k);
                    k--;
                }
            }
            ImGui::PopID();
        }
        ImGui::Unindent();
        ImGui::Spacing();
    }
    ImGui::End();
}

void Editor::ShowAnimationEditorWindow()
{
    if (!this->show_animation_editor_window) return;

    ImGui::Begin("Animation Editor", &this->show_animation_editor_window, ImGuiWindowFlags_MenuBar);

    // --- 顶部全局控制 ---
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Close Window")) this->show_animation_editor_window = false;
        ImGui::EndMenuBar();
    }

    // 播放控制
    static bool is_global_play = false;

    if (ImGui::Button(is_global_play ? "Stop All" : "Play All"))
    {
        is_global_play = !is_global_play;
        for (size_t i = 0; i < obj_anim_states.size(); i++)
        {
            if (obj_anim_states[i].is_enabled)
            {
                obj_anim_states[i].is_playing = is_global_play;
                // 只有在开始播放时才重置时间，暂停不重置
                if (is_global_play) obj_anim_states[i].current_time = 0.0f;
            }
        }
    }
    ImGui::Separator();

    // --- 布局开始 ---
    // 左边列表，右边详情
    static int selected_idx = -1;

    // 【关键修复】左窗口必须在使用 SameLine 之前关闭
    // 参数定义: ID, Size(宽, 高), Border, Flags
    if (ImGui::BeginChild("##ObjList", ImVec2(220, 0), true))
    {
        for (size_t i = 0; i < obj_anim_clips.size(); i++)
        {
            auto& state = obj_anim_states[i];
            std::string label = "Obj " + std::to_string(i);
            if (!state.is_enabled) label += " [Off]";

            ImGui::PushID(i); // 确保 ID 唯一
            if (ImGui::Selectable(label.c_str(), (selected_idx == (int)i)))
            {
                selected_idx = (int)i;
            }

            // 显示播放状态的小图标
            if (state.is_playing)
            {
                ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20);
                ImGui::TextColored(ImVec4(0, 1, 0, 1), ">");
            }
            ImGui::PopID();
        }
    }
    // 【关键修复】立即关闭左子窗口
    ImGui::EndChild();

    // 【关键修复】SameLine 必须在两个 EndChild 之间
    ImGui::SameLine();

    // 【关键修复】右窗口
    if (ImGui::BeginChild("##Details", ImVec2(0, 0), true))
    {
        if (selected_idx >= 0 && selected_idx < (int)obj_anim_clips.size())
        {
            auto& clip = obj_anim_clips[selected_idx];
            auto& state = obj_anim_states[selected_idx];

            // --- 标题栏控制 ---
            ImGui::Checkbox("Enable", &state.is_enabled);
            ImGui::SameLine();
            if (ImGui::Button(state.is_playing ? "Pause" : "Play"))
            {
                state.is_playing = !state.is_playing;
                if (state.is_playing) state.current_time = 0.0f;
            }

            ImGui::Separator();

            // --- 更新总时长拖动条 ---
            // 拖动 Duration 会自动更新最后一个关键帧的时间
            if (ImGui::DragFloat("Total Duration", &clip.duration, 0.1f, 0.1f, 60.0f, "%.1fs"))
            {
                if (clip.positionTrack.keys.size() >= 2) clip.positionTrack.keys.back().time = clip.duration;
                if (clip.rotationTrack.keys.size() >= 2) clip.rotationTrack.keys.back().time = clip.duration;
                if (clip.scaleTrack.keys.size() >= 2)    clip.scaleTrack.keys.back().time = clip.duration;
            }

            // --- 预览时间轴 ---
            // 这里使用 current_time 作为预览时间，如果不播放，可以直接拖拽这个滑块
            if (!state.is_playing)
            {
                // 预览模式逻辑保持不变
                if (ImGui::DragFloat("Preview Time", &state.current_time, 0.01f, 0.0f, clip.duration, "%.2fs"))
                {
                    obj_transforms[selected_idx].Position = clip.positionTrack.Sample(state.current_time);
                    // ...
                }
            }
            else
            {
                // --- 【修改点】根据不同模式计算显示进度 ---
                float display_progress = 0.0f;
                if (clip.playMode == AnimPlayMode::Once)
                {
                    // 单次播放：就是时间比例（但不要超过1）
                    display_progress = state.current_time / clip.duration;
                    if (display_progress > 1.0f) display_progress = 1.0f;
                }
                else if (clip.playMode == AnimPlayMode::Loop)
                {
                    // 循环播放：取余数就是进度
                    display_progress = fmod(state.current_time, clip.duration) / clip.duration;
                }
                else if (clip.playMode == AnimPlayMode::PingPong)
                {
                    // PingPong：三角波逻辑
                    // 周期是 2 * duration
                    float cycle = fmod(state.current_time, clip.duration * 2.0f);

                    if (cycle <= clip.duration)
                    {
                        // 前半段：正向 (0 -> 1)
                        display_progress = cycle / clip.duration;
                    }
                    else
                    {
                        // 后半段：反向 (1 -> 0)
                        display_progress = 1.0f - (cycle - clip.duration) / clip.duration;
                    }
                }
                // 显示文本
                ImGui::Text("Playing... %.2fs / %.2fs", state.current_time, clip.duration);

                // 使用计算出来的 display_progress 绘制进度条
                ImGui::ProgressBar(display_progress, ImVec2(0,0));
            }

            ImGui::SeparatorText("Keyframes");

            // 播放模式
            int mode = static_cast<int>(clip.playMode);
            const char* modes[] = { "Once", "Loop", "PingPong" };
            if (ImGui::Combo("Mode", &mode, modes, 3))
            {
                clip.playMode = static_cast<AnimPlayMode>(mode);
            }

            ImGui::Spacing();

            // --- 位置轨道 ---
            if (ImGui::CollapsingHeader("Position Track", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Add Pos Key at Current"))
                {
                    // 在当前时间轴位置添加一个关键帧，值为当前物体的实际位置
                    clip.positionTrack.AddKey(state.current_time, obj_transforms[selected_idx].Position);
                    // 如果时间超出了当前总时长，自动延长时长
                    if (state.current_time > clip.duration) clip.duration = state.current_time;
                }

                if (ImGui::Button("Clear All Pos Keys")) clip.positionTrack.keys.clear();

                ImGui::Indent();
                // 关键帧列表
                for (size_t k = 0; k < clip.positionTrack.keys.size(); k++)
                {
                    auto& key = clip.positionTrack.keys[k];
                    ImGui::PushID(k);

                    // 第一行：拖拽时间
                    ImGui::SetNextItemWidth(60);
                    if (ImGui::DragFloat("T", &key.time, 0.01f, 0.0f, 60.0f, "%.1f"))
                    {
                        // 如果拖改变最大时间，同步更新总时长
                        if (key.time > clip.duration) clip.duration = key.time;
                    }
                    ImGui::SameLine();

                    // 第一行：拖拽数值 和 删除按钮
                    ImGui::SetNextItemWidth(180);
                    ImGui::DragFloat3("Position", &key.value.x, 0.1f);
                    ImGui::SameLine();

                    if (clip.positionTrack.keys.size() > 1) // 至少保留一个
                    {
                        if (ImGui::SmallButton("Del"))
                        {
                            clip.positionTrack.keys.erase(clip.positionTrack.keys.begin() + k);
                            k--; // 删除后索引回退
                        }
                    }
                    ImGui::PopID();
                }
                ImGui::Unindent();
                ImGui::Spacing();
            }

            // --- 旋转轨道 ---
            if (ImGui::CollapsingHeader("Rotation Track", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Add Rot Key at Current"))
                {
                    clip.rotationTrack.AddKey(state.current_time, obj_transforms[selected_idx].Rotation);
                    if (state.current_time > clip.duration) clip.duration = state.current_time;
                }

                if (ImGui::Button("Clear All Rot Keys")) clip.rotationTrack.keys.clear();

                ImGui::Indent();
                for (size_t k = 0; k < clip.rotationTrack.keys.size(); k++)
                {
                    auto& key = clip.rotationTrack.keys[k];
                    ImGui::PushID(k + 1000);
                    ImGui::SetNextItemWidth(60);
                    if (ImGui::DragFloat("T", &key.time, 0.01f, 0.0f, 60.0f, "%.1f"))
                    {
                        if (key.time > clip.duration) clip.duration = key.time;
                    }
                    ImGui::SameLine();

                    ImGui::SetNextItemWidth(180);
                    // 注意：旋转通常用角度显示，如果你的 Lerp 内部用弧度，这里要注意单位一致性
                    // 假设你的 obj_transforms.Rotation 是度数，那你这里直接显示就行
                    ImGui::DragFloat3("Rotation", &key.value.x, 1.0f);
                    ImGui::SameLine();

                    if (clip.rotationTrack.keys.size() > 1)
                    {
                        if (ImGui::SmallButton("Del"))
                        {
                            clip.rotationTrack.keys.erase(clip.rotationTrack.keys.begin() + k);
                            k--;
                        }
                    }
                    ImGui::PopID();
                }
                ImGui::Unindent();
                ImGui::Spacing();
            }

             // --- 缩放轨道 ---
            if (ImGui::CollapsingHeader("Scale Track", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::Button("Add Scale Key at Current"))
                {
                    clip.scaleTrack.AddKey(state.current_time, obj_transforms[selected_idx].Scale);
                    if (state.current_time > clip.duration) clip.duration = state.current_time;
                }

                if (ImGui::Button("Clear All Scale Keys")) clip.scaleTrack.keys.clear();

                ImGui::Indent();
                for (size_t k = 0; k < clip.scaleTrack.keys.size(); k++)
                {
                    auto& key = clip.scaleTrack.keys[k];
                    ImGui::PushID(k + 2000);
                    ImGui::SetNextItemWidth(60);
                    if (ImGui::DragFloat("T", &key.time, 0.01f, 0.0f, 60.0f, "%.1f"))
                    {
                        if (key.time > clip.duration) clip.duration = key.time;
                    }
                    ImGui::SameLine();

                    ImGui::SetNextItemWidth(180);
                    ImGui::DragFloat3("Scale", &key.value.x, 0.01f);
                    ImGui::SameLine();

                    if (clip.scaleTrack.keys.size() > 1)
                    {
                        if (ImGui::SmallButton("Del"))
                        {
                            clip.scaleTrack.keys.erase(clip.scaleTrack.keys.begin() + k);
                            k--;
                        }
                    }
                    ImGui::PopID();
                }
                ImGui::Unindent();
                ImGui::Spacing();
            }
        }
        else
        {
            ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Select an object from the left list.");
        }
    } // 关闭右侧详情窗口
    ImGui::EndChild();

    // 关闭总窗口
    ImGui::End();
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
        const auto& new_obj = scene.game_object_list.at(scene.game_object_list.size() - 1);
        // 同步 Transform
        this->obj_transforms.push_back(new_obj.transform);
        // 【修改】同步添加动画数据
        CS::AnimationClip clip;
        CS::AnimPlayerState state;

        // 默认动画：原地不动，持续1秒
        auto& t = new_obj.transform;
        // 位置轨道
        clip.positionTrack.AddKey(0.0f, t.Position);
        clip.positionTrack.AddKey(1.0f, t.Position);
        // 旋转轨道
        clip.rotationTrack.AddKey(0.0f, t.Rotation);
        clip.rotationTrack.AddKey(1.0f, t.Rotation);
        // 缩放轨道
        clip.scaleTrack.AddKey(0.0f, t.Scale);
        clip.scaleTrack.AddKey(1.0f, t.Scale);

        clip.duration = 1.0f;

        this->obj_anim_clips.push_back(clip);
        this->obj_anim_states.push_back(state);
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

void Editor::UpdateAnimationSystem()
{
    float dt = io.DeltaTime;

    if (camera_anim_state.is_enabled && camera_anim_state.is_playing)
    {
        camera_anim_state.current_time += dt * camera_anim_state.speed;
        float dur = camera_anim_clip.duration;
        float sample_time = 0.0f;
        if (dur <= 0.0001f) dur = 0.0001f;
        // 模式计算 (和对象动画逻辑一致)
        switch (camera_anim_clip.playMode)
        {
            case AnimPlayMode::Once:
                if (camera_anim_state.current_time >= dur)
                {
                    camera_anim_state.current_time = dur;
                    camera_anim_state.is_playing = false; // 停止
                }
                sample_time = camera_anim_state.current_time;
                break;
            case AnimPlayMode::Loop:
                sample_time = fmod(camera_anim_state.current_time, dur);
                break;
            case AnimPlayMode::PingPong:
                float cycle_time = fmod(camera_anim_state.current_time, dur * 2.0f);
                if (cycle_time < 0) cycle_time += dur * 2.0f;
                if (cycle_time <= dur) sample_time = cycle_time;
                else sample_time = dur * 2.0f - cycle_time;
                break;
        }
        // 采样并应用到摄像机
        this->camera_eye_position = camera_anim_clip.eyeTrack.Sample(sample_time);
        this->camera_lookat_position = camera_anim_clip.lookAtTrack.Sample(sample_time);
    }

    // --- 2. 对象动画逻辑 (全新多关键帧系统) ---
    if (obj_anim_clips.size() != obj_transforms.size()) return;

    for (size_t i = 0; i < obj_anim_clips.size(); i++)
    {
        auto& clip = obj_anim_clips[i];
        auto& state = obj_anim_states[i];

        // 如果未启用或未播放，跳过
        if (!state.is_enabled || !state.is_playing) continue;

        // 1. 累加时间
        state.current_time += dt * state.speed;

        // 2. 模式处理与采样时间计算
        float sample_time = 0.0f;
        float dur = clip.duration;

        // 防止除零
        if (dur <= 0.0001f) dur = 0.0001f;

        switch (clip.playMode)
        {
        case AnimPlayMode::Once:
            if (state.current_time >= dur)
            {
                state.current_time = dur; // 停在最后一帧
                state.is_playing = false;
            }
            sample_time = state.current_time;
            break;

        case AnimPlayMode::Loop:
            sample_time = fmod(state.current_time, dur); // 取余数循环
            break;

        case AnimPlayMode::PingPong:
        {
            // PingPong 逻辑：时间在 0 -> dur -> 0 之间振荡
            float cycle_time = fmod(state.current_time, dur * 2.0f);
            if (cycle_time < 0) cycle_time += dur * 2.0f;

            if (cycle_time <= dur)
                sample_time = cycle_time; // 正向
            else
                sample_time = dur * 2.0f - cycle_time; // 反向
            break;
        }
        default:
            sample_time = state.current_time;
            break;
        }

        // 3. 应用采样数据到 Transform
        // 这里的 Sample 函数会自动根据 sample_time 在所有关键帧里插值
        obj_transforms[i].Position = clip.positionTrack.Sample(sample_time);
        obj_transforms[i].Rotation = clip.rotationTrack.Sample(sample_time);
        obj_transforms[i].Scale    = clip.scaleTrack.Sample(sample_time);
    }
}

void Editor::PlayAllAnimations(bool play)
{
    // --- 1. 处理摄像机 ---
    // 只有启用时才响应全局播放
    if (camera_anim_state.is_enabled)
    {
        camera_anim_state.is_playing = play;
        if (play) camera_anim_state.current_time = 0.0f;
    }

    // --- 2. 处理所有对象 ---
    for (auto& state : obj_anim_states)
    {
        if (state.is_enabled)
        {
            state.is_playing = play;
            if (play) state.current_time = 0.0f;
        }
    }
}

bool Editor::get_exit_the_program() const
{
    return this->exit_the_program;
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
