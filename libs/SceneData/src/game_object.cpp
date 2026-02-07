//
// Created by xiaoxuetu on 2026/1/30.
//

#include "SceneData/game_object.h"

#include <fstream>
#include <limits>
#include <filesystem>
#include <format>

namespace CS::SceneData
{
GameObject::GameObject(const std::filesystem::path& obj_file_name, const std::filesystem::path& glyph_map_file_name)
{
    this->LoadGamebjectFromFile(obj_file_name, glyph_map_file_name);
}

void GameObject::LoadGamebjectFromFile(const std::filesystem::path& obj_file_name,
                                       const std::filesystem::path& glyph_map_file_name)
{
    this->LoadObj(obj_file_name);
    this->LoadGlyphMap(glyph_map_file_name);
}

void GameObject::LoadObj(const std::filesystem::path& obj_file_name)
{
    this->mesh.Vertices.clear();
    this->mesh.Indices.clear();
    this->mesh.MaterialName.clear();

    std::ifstream obj_file(obj_file_name);

    if (!obj_file.is_open())
    {
        throw std::logic_error{std::format("Failed to read the {} file", obj_file_name.string().c_str()).c_str()};
    }

    std::string material_name;
    Math::Vector3d vertex{};
    std::string str;

    while (obj_file >> str)
    {
        /* 读取顶点 */
        if (str == "v")
        {
            obj_file >> vertex.x;
            obj_file >> vertex.y;
            obj_file >> vertex.z;
            this->mesh.Vertices.push_back(vertex);
        }
        /* 指定接下来读取的面材质 */
        else if (str == "usemtl")
        {
            obj_file >> str;
            material_name = str;
        }
        /* 读取面索引 */
        else if (str == "f")
        {
            unsigned int index = 0;

            obj_file >> index;
            this->mesh.Indices.push_back(index - 1);
            obj_file >> index;
            this->mesh.Indices.push_back(index - 1);
            obj_file >> index;
            this->mesh.Indices.push_back(index - 1);

            this->mesh.MaterialName.push_back(material_name);
        }
        else if (str == "mtllib")
        {
            obj_file >> str;

            /* 计算材质所在的路径 */
            const std::filesystem::path& p(obj_file_name);
            std::filesystem::path dir = p.parent_path();

            if (dir.empty())
                dir = "."; //当前目录

            /* 加载材质文件 */
            this->material_manager.LoadMaterialFile((dir / str).string().c_str());
        }
        else
        {
            obj_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void GameObject::LoadGlyphMap(const std::filesystem::path& glyph_map_file_name)
{
    this->material_manager.LoadGlyphMap(glyph_map_file_name);
}
}
