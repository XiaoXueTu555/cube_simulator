//
// Created by xiaoxuetu on 2026/1/27.
//

#ifndef SCENEDATA_MESH_H
#define SCENEDATA_MESH_H

#include <vector>

#include <string>
#include "transform.h"


namespace CS::SceneData
{

/*
 * Mesh
 * 网格体
 */
class Mesh
{
public:
    /* 顶点池 */
    std::vector<Math::Vector3d> Vertices;

    /* 索引池
     * 每三个索引描述了一个三角形
     * 每个索引指向了顶点池中的一个点
     */
    std::vector<unsigned int> Indices;

    /*
     * 每个面对应的材质名
     */
    std::vector<std::string> MaterialName;

    Mesh() = default;
};

}


#endif //SCENEDATA_MESH_H
