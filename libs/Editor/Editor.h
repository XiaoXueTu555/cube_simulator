//
// Created by xiaoxuetu on 2026/2/6.
//

#ifndef CUBE_SIMULATOR_EDITOR_H
#define CUBE_SIMULATOR_EDITOR_H

#include <vector>

#include "SceneData/camera.h"
#include "SceneData/transform.h"
#include "SceneData/scene.h"
#include "Renderer/view_port.h"
#include "Renderer/scene_renderer.h"
#include "imgui.h"

namespace CS
{
class Editor
{
public:
    /* 显示View port窗口，并渲染一个新的帧 */
    void ShowViewPortWindow(const Renderer::Viewport& port, const ImGuiIO& io);

    /* 显示Editor窗口 */
    void ShowEditorWindow(const ImGuiIO& io, Renderer::SceneRenderer& renderer, SceneData::scene& scene,
                          Renderer::Viewport& port);

    /* 显示摄像机细节窗口 */
    void ShowCameraWindow();

    /* 显示GameObject细节窗口 */
    void ShowGameObjectWindow(SceneData::scene& scene);

    /* 显示添加GameObject窗口 */
    void ShowAddGameObjectWindow(SceneData::scene& scene);

public:
    float get_camera_near() const;
    float get_camera_far() const;
    float get_camera_fov() const;
    float get_camera_aspect_ratio() const;
    const std::vector<SceneData::Transform>& get_obj_transforms() const;

    /* 所有的编辑数据 */
private:
    //view port 显示字体的大小
    float port_font_size_base = 12.0f;

    //摄像机近裁剪面距离、远裁剪面距离、视场角、宽高比
    float camera_near = 1;
    float camera_far = 600;
    float camera_fov = 60; //角度
    float camera_aspect_ratio = 16.0f / 9.0f;

    // 摄像机视角参数
    Math::Vector3d camera_eye_position{0, 0, 30};
    Math::Vector3d camera_lookat_position{0, 0, 0};

    //每个对象的transform
    std::vector<CS::SceneData::Transform> obj_transforms;

    /* 窗口控制变量 */
private:
    bool show_view_port_window = true;
    bool show_camera_window = false;
    bool show_game_object_window = false;
    bool show_add_game_object_window = false;
};
} // CS

#endif //CUBE_SIMULATOR_EDITOR_H
