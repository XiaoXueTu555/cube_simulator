//
// Created by xiaoxuetu on 2026/1/31.
//

#ifndef SCENEDATA_SCENE_H
#define SCENEDATA_SCENE_H
#include <vector>

#include "camera.h"
#include "game_object.h"

namespace CS::SceneData
{

/*
 * 场景类
 * 描述场景中的所有物体以及摄像机
 */
class scene
{
public:
    scene() : game_object_list{}, camera {} {}

    /*
     * 向场景中添加一个GameObject
     */
    template<typename ... Args>
    void AddGameObject(Args&& ... args);
public:
    std::vector<GameObject> game_object_list;
    Camera camera;
};


template<typename ... Args>
void scene::AddGameObject(Args&&... args)
{
    this->game_object_list.emplace_back(std::forward<Args>(args) ...);
}

}

#endif //SCENEDATA_SCENE_H
