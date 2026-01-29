//
// Created by xiaoxuetu on 2026/1/28.
//

#ifndef SCENEDATA_MODEL_H
#define SCENEDATA_MODEL_H

#include "mesh.h"
#include "material_manager.h"

#include <vector>

#include "transform.h"

namespace CS::SceneData
{

/* 模型类 */
class Model
{
public:
    /* 基础网格体 */
    std::vector<Mesh> static_meshs;

    /* 材质映射表 */
    MaterialManager material_manager;

    /* 变换 */
    Transform transform;

    Model() = default;

    /* 加载obj文件作为模型 */
    bool LoadSingleMeshAsModel(const char* obj_filename);

    /*
     * 加载模型
     * 加载所有的 Mesh
     */
    virtual bool LoadModel();

    virtual ~Model();
};

}

#endif //SCENEDATA_MODEL_H
