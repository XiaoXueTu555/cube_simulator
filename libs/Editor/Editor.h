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
    void ShowEditorWindow(const SceneData::scene& scene);

    /* 显示摄像机细节窗口 */
    void ShowCameraWindow();

    /* 显示GameObject细节窗口 */
    void ShowGameObjectWindow(const SceneData::scene& scene, int index);

    /* 所有的编辑数据 */
private:
    //view port 显示字体的大小
    float port_font_size_base = 12.0f;

    /* camera */
    CS::SceneData::Camera camera;

    //每个对象的transform
    std::vector<CS::SceneData::Transform> obj_transforms;

    //每个对象的顶点数
    std::vector<int> obj_vertex_count;

    //每个对象的三角面数
    std::vector<int> obj_triangle_count;

    /* 窗口控制变量 */
private:
    bool show_view_port_window = true;
    bool show_camera_window = false;
    bool show_game_object_window = false;

};
} // CS

#endif //CUBE_SIMULATOR_EDITOR_H
