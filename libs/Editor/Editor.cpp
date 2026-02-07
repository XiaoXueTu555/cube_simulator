//
// Created by xiaoxuetu on 2026/2/6.
//

#include "Editor.h"
#include <format>

namespace CS
{
void Editor::ShowViewPortWindow(const Renderer::Viewport& port, const ImGuiIO& io)
{
    ImGui::Begin("game View Port Window", &this->show_view_port_window);
    ImGui::End();
}

void Editor::ShowEditorWindow(const SceneData::scene& scene)
{

}

void Editor::ShowCameraWindow()
{
    ImGui::Begin("Camera Window", &this->show_camera_window);
    ImGui::DragFloat("Camera Near",&camera_near,CS_KINDA_SMALL_NUMBER,5);
    ImGui::DragFloat("Camera Far",&camera_far,CS_KINDA_SMALL_NUMBER,1000);
    ImGui::DragFloat("Camera FOV",&camera_fov,CS_AngleToRadian(34.0f),CS_AngleToRadian(71.0f));
    ImGui::Text("Camera Aspect Ratio: 16.0f / 9.0f");
    ImGui::End();
}

void Editor::ShowGameObjectWindow(SceneData::scene& scene, int index)
{
    ImGui::Begin("Game Object Window", &this->show_game_object_window);

    ImGui::SeparatorText("Transform");
    if (ImGui::TreeNode("Scale"))
    {
        ImGui::DragFloat("XScale",&obj_transforms[index].Scale.x,0.01,100);
        ImGui::DragFloat("YScale",&obj_transforms[index].Scale.y,0.01,100);
        ImGui::DragFloat("ZScale",&obj_transforms[index].Scale.z,0.01,100);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Rotation"))
    {
        ImGui::DragFloat("XRotation", &obj_transforms[index].Rotation.x,-360,360);
        ImGui::DragFloat("YRotation", &obj_transforms[index].Rotation.y,-360,360);
        ImGui::DragFloat("ZRotation", &obj_transforms[index].Rotation.z,-360,360);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Position"))
    {
        ImGui::DragFloat("XPosition", &obj_transforms[index].Position.x,-100,100);
        ImGui::DragFloat("YPosition", &obj_transforms[index].Position.y,-100,100);
        ImGui::DragFloat("ZPosition", &obj_transforms[index].Position.z,-100,100);
        ImGui::TreePop();
    }

    ImGui::Text("");
    ImGui::SeparatorText("Other information");
    if (ImGui::TreeNode("Vertice"))
    {
        int vetrix_count = scene.game_object_list[index].mesh.Vertices.size();
        ImGui::Text("vertex_count: %d", &vetrix_count);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Triangle"))
    {
        int trianged_count = scene.game_object_list[index].mesh.Indices.size() / 3;
        ImGui::Text("triangle_count: %d", &trianged_count);
        ImGui::TreePop();
    }

    ImGui::End();
}

} // CS
