//
// Created by xiaoxuetu on 2026/1/28.
//

#ifndef SCENEDATA_MODEL_H
#define SCENEDATA_MODEL_H

#include "mesh.h"
#include "material_manager.h"
#include "transform.h"

#include <filesystem>

namespace CS::SceneData
{

/* 游戏场景的实体对象 */
class GameObject
{
public:
    /* 基础网格体 */
    Mesh mesh;

    /* 材质映射表 */
    MaterialManager material_manager;

    /* 变换 */
    Transform transform;

    GameObject();
    GameObject(const std::filesystem::path& obj_file_name, const std::filesystem::path& glyph_map_file_name);

    /*
     * 加载模型文件
     * obj
     * mtl
     * yaml
     */
    void LoadGamebjectFromFile(const std::filesystem::path& obj_file_name,
                               const std::filesystem::path& glyph_map_file_name);

private:
    /*
     * 加载obj文件
     * 该函数会通过指定的obj文件加载模型数据，
     * 若obj中编码材质文件，会在obj文件同目录下自动读取。
     */
    void LoadObj(const std::filesystem::path& obj_file_name);

    /*
     * 加载材质-字符映射文件
     */
    void LoadGlyphMap(const std::filesystem::path& glyph_map_file_name);
};

}

#endif //SCENEDATA_MODEL_H
