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
    std::vector<Math::Vector3d> Vertexs;

    /* 索引池
     * 每三个索引描述了一个三角形
     * 每个索引指向了顶点池中的一个点
     */
    std::vector<unsigned int> Indexs;

    /*
     * 每个面对应的材质名
     */
    std::vector<std::string> MaterialName;

    /* 变换 */
    Transform transform;

    Mesh();
    explicit Mesh(const char* filename);

    /*
     * 加载OBJ模型文件
     * 读取obj中的顶点坐标数据(v)、面数据(f)
     * 读取obj中的材质数据（若有）：
          面绑定的材质 (usemtl Material)
          mtl文件名字 (mtllib Model.mtl)
     *
     */
    bool LoadObj(const char* filename);
};

}


#endif //SCENEDATA_MESH_H
