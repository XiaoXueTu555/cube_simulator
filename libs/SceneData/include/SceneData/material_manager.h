//
// Created by xiaoxuetu on 2026/1/28.
//

#ifndef SCENEDATA_MATERIAL_GLYPH_MAPPER_H
#define SCENEDATA_MATERIAL_GLYPH_MAPPER_H

#include <map>
#include <string>
#include <filesystem>

#include "Math/vector3d.h"

namespace CS::SceneData
{

/*
 * 材质管理器
 * 建立面材质与面颜色值、面材质与面字符的映射表
 */
class MaterialManager
{
private:
    /* 材质字符映射表 */
    std::map<std::string, char> material_glyph_mapper;

    /* 材质颜色映射表 */
    std::map<std::string, Math::Vector3d> material_color_mapper;
public:
    MaterialManager() = default;
    MaterialManager(const std::filesystem::path& mtl_file, const std::filesystem::path& glyph_map_file);

    /*
     * 加载材质-字符映射文件
     * 文件后缀：glyph_mappings.yaml
     * 文件格式如下：
     * MaterialName1 : #
     * MaterialName2 : *
     */
    void LoadGlyphMap(const std::filesystem::path& filename);

    /*
     * 加载材质文件(.mtl)
     * 读取材质的颜色值
     */
    void LoadMaterialFile(const std::filesystem::path& filename);

    /* 从映射表中取出对应的字符
     * 若未在映射表中查找到材质则返回特定字符 '*'
     */
    char GetChar(const std::string& MaterialName) const;

    /* 从映射表中取出对应的字符
     * 若未在映射表中查找到材质则返回特定颜色 (1, 1, 1)
     */
    Math::Vector3d GetColor(const std::string& MaterialName) const;
};

}

#endif //SCENEDATA_MATERIAL_GLYPH_MAPPER_H
