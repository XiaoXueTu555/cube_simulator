//
// Created by xiaoxuetu on 2026/2/6.
//

#ifndef CUBE_SIMULATOR_EDITOR_H
#define CUBE_SIMULATOR_EDITOR_H

#include <vector>

#include "SceneData/transform.h"
#include "SceneData/scene.h"
#include "Renderer/view_port.h"
#include "Renderer/scene_renderer.h"

#include "imgui.h"

#include "animation.h"

namespace CS
{
class Editor
{
public:
    Editor(ImGuiIO& io, SceneData::scene& scene, Renderer::SceneRenderer& renderer, Renderer::Viewport& port);
    ~Editor();

    /* 显示View port窗口，并渲染一个新的帧 */
    void ShowViewPortWindow();

    /* 显示Editor窗口 */
    void ShowEditorWindow();

    /* 显示摄像机细节窗口 */
    void ShowCameraWindow();

    /* 显示GameObject细节窗口 */
    void ShowGameObjectWindow();

    /* 显示添加GameObject窗口 */
    void ShowAddGameObjectWindow();

    /* 全局快捷键 */
    void GlobalShortcut();

    /* 动画编辑窗口 */
public:
    /* 展示摄像机动画编辑窗口 */
    void ShowCameraAnimWindow();

    /* 显示GameObject动画编辑窗口 */
    void ShowAnimationEditorWindow();

public:

    /* 鼠标键盘 移动场景摄像机 */
    void MoveSceneCamera();

    void AddGameObjectFormFiles(const char* obj_filename, const char* yaml_filename);

    /* 保存最近打开的模型文件 */
    void SaveRecentlyOpenedFiles() const;

    /* 加载保存文件 */
    void LoadSaveFile();

    /* 系统更新 */
    void UpdateAnimationSystem();

    /* 播放/停止所有已启用的动画 */
    void PlayAllAnimations(bool play);

public:
    float get_camera_near() const;
    float get_camera_far() const;
    float get_camera_fov() const;
    float get_camera_aspect_ratio() const;

    const Math::Vector3d& get_camera_eye_position() const;
    const Math::Vector3d& get_camera_lookat_position() const;

    const std::vector<SceneData::Transform>& get_obj_transforms() const;

    /* 用户是否关闭程序 */
    bool get_exit_the_program() const;
public:
    /*
     * 【移动】WASD 逻辑
     * forward: 前进距离 (+前进, -后退)
     * right:   右移距离 (+右, -左)
     * up:      上升距离 (+上, -下)
     */
    void MoveCamera(float forward, float right, float up);

    /*
     * 【旋转】鼠标视角逻辑
     * yaw:   水平旋转角度 (偏航)
     * pitch: 垂直旋转角度 (俯仰)
     */
    void RotateView(float yaw, float pitch, bool use_radian = false);

private:
    /* 外部的场景和渲染器 */
    ImGuiIO& io;
    SceneData::scene& scene;
    Renderer::SceneRenderer& renderer;
    Renderer::Viewport& port;


    /* 所有的编辑数据 */
    float port_font_size_base = 12.0f; //view port 显示字体的大小

    //摄像机近裁剪面距离、远裁剪面距离、视场角、宽高比
    float camera_near = 0.2;
    float camera_far = 600;
    float camera_fov = 60; //角度
    float camera_aspect_ratio = 16.0f / 9.0f;

    // 摄像机视角参数
    Math::Vector3d camera_eye_position{0, 0, 10};
    Math::Vector3d camera_lookat_position{0, 0, 0};
    Math::Vector3d camera_up_direction = Math::Vector3d::UpVector;

    /* view port窗口中移动摄像机的速度 */
    float camera_forward_speed      = 3; // W,S 每秒移动距离
    float camera_right_speed        = 3; // A,D 每秒移动距离
    float camera_up_speed           = 3; // Q,E 每秒移动距离
    float camera_yaw_sensitivity    = 0.001; // 水平旋转灵敏度 (偏航)，每像素移动多少度
    float camera_pitch_sensitivity  = 0.001; // 垂直旋转灵敏度 (俯仰)，每像素移动多少度


    //每个对象的transform
    std::vector<CS::SceneData::Transform> obj_transforms;

    CameraAnimClip camera_anim_clip;   // 包含 Eye 和 LookAt 两条轨道
    AnimPlayerState camera_anim_state; // 播放状态
    std::vector<CS::AnimationClip> obj_anim_clips;      // 动画数据 (位置、旋转等)
    std::vector<CS::AnimPlayerState> obj_anim_states;   // 播放状态


    /* 窗口控制变量 */
    bool show_view_port_window = true;
    bool show_camera_window = false;
    bool show_game_object_window = false;
    bool show_add_game_object_window = false;

    /* 动画窗口 */
    bool show_camera_anim_window = false;
    bool show_animation_editor_window = false;

    /* imgui的Debug Log窗口 */
    bool show_imgui_debug_log_window = false;

    /* 编辑器保存的其他数据 */
    std::string recently_opened_obj_files[5];
    std::string recently_opened_yaml_files[5];
    bool exit_the_program = false;
};
} // CS

#endif //CUBE_SIMULATOR_EDITOR_H
