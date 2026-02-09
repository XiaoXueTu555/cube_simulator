//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/material_manager.h"

#include <fstream>
#include <limits>
#include <format>

namespace CS::SceneData
{
MaterialManager::MaterialManager(const std::filesystem::path& mtl_file, const std::filesystem::path& glyph_map_file)
{
    this->LoadMaterialFile(mtl_file);
    this->LoadGlyphMap(glyph_map_file);
}

void MaterialManager::LoadGlyphMap(const std::filesystem::path& filename)
{
    std::ifstream map_file(filename);

    if (!map_file.is_open())
    {
        throw std::logic_error{std::format("Failed to read the {} file", filename.string().c_str()).c_str()};
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

    map_file.close();
}

void MaterialManager::LoadMaterialFile(const std::filesystem::path& filename)
{
    std::ifstream mtl_file(filename);

    if (!mtl_file.is_open())
    {
        throw std::logic_error{std::format("Failed to read the {} file", filename.string().c_str()).c_str()};
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

    mtl_file.close();
}

char MaterialManager::GetChar(const std::string& MaterialName) const
{
    auto it = this->material_glyph_mapper.find(MaterialName);

    if (it == this->material_glyph_mapper.end())
        return '*';

    return it->second;
}

Math::Vector3d MaterialManager::GetColor(const std::string& MaterialName) const
{
    auto it = this->material_color_mapper.find(MaterialName);

    if (it == this->material_color_mapper.end())
        return Math::Vector3d{1, 1, 1};

    return it->second;
}
}
