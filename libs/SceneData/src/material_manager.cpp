//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/material_manager.h"

#include <fstream>
#include <limits>
#include <format>

namespace CS::SceneData
{
MaterialManager::MaterialManager(const char* mtl_file, const char* glyph_map_file)
{
    if (!this->LoadMaterialFile(mtl_file))
    {
        throw std::invalid_argument(std::format("{} init: read fail from {} file.", "Mesh", mtl_file));
    }
    if (!this->LoadGlyphMap(glyph_map_file))
    {
        throw std::invalid_argument(std::format("{} init: read fail from {} file.", "Mesh", glyph_map_file));
    }
}

bool MaterialManager::LoadGlyphMap(const char* filename)
{
    std::ifstream map_file(filename);

    if (!map_file.is_open())
    {
        return false;
    }

    std::string mtl_name;
    std::string str;
    while (map_file >> str)
    {
        mtl_name = str;

        map_file >> str;
        map_file >> str;
        /* 只映射第一个字符 */
        this->material_glyph_mapper.insert({mtl_name, str.c_str()[0]});
    }

    return true;
}

bool MaterialManager::LoadMaterialFile(const char* filename)
{
    std::ifstream mtl_file(filename);

    if (!mtl_file.is_open())
    {
        return false;
    }

    /* 临时变量 */
    std::string mtl_name;
    Math::Vector3d mtl_color{};

    std::string str;

    while (mtl_file >> str)
    {
        /* 读取材质 */
        if (str == "newmtl")
        {
            mtl_file >> mtl_name;
        }
        /* 读取颜色值 */
        else if (str == "Kd")
        {
            mtl_file >> mtl_color.x;
            mtl_file >> mtl_color.y;
            mtl_file >> mtl_color.z;

            this->material_color_mapper.insert({mtl_name, mtl_color});
        }
        else
        {
            mtl_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return true;
}

char MaterialManager::GetChar(const std::string& MaterialName) const
{
    auto it = this->material_glyph_mapper.find(MaterialName);

    if (it == this->material_glyph_mapper.end())
        return '*';

    return it->second;
}
}
