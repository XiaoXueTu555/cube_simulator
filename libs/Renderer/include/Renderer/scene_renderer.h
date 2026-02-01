//
// Created by xiaoxuetu on 2026/1/31.
//

#ifndef CUBE_SIMULATOR_RENDERER_H
#define CUBE_SIMULATOR_RENDERER_H

#include "SceneData/scene.h"
#include "Renderer/view_port.h"

namespace CS::Renderer
{

/*
 * 场景渲染器
 */
class SceneRenderer
{
public:
    SceneRenderer() = default;

    void Renderer(const SceneData::scene& game_scene, Viewport& view_port);
};

}

#endif //CUBE_SIMULATOR_RENDERER_H
