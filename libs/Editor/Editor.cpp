//
// Created by xiaoxuetu on 2026/2/6.
//

#include "Editor.h"
#include <format>

namespace CS {
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

}

void Editor::ShowGameObjectWindow(const SceneData::scene& scene, int index)
{

}
} // CS
