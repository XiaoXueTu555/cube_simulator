//
// Created by xiaoxuetu on 2026/1/29.
//

#include "SceneData/mesh.h"
#include <fstream>

/* std::numeric_limits */
#include <limits>
#include <format>


namespace CS::SceneData
{
Mesh::Mesh() : transform{} {}

Mesh::Mesh(const char* filename)
    : transform{}
{
    if (!this->LoadObj(filename))
    {
        throw std::invalid_argument(std::format("{} init: read fail from {} file.", "Mesh", filename));
    }
}

bool Mesh::LoadObj(const char* filename)
{
    this->Vertexs.clear();
    this->Indexs.clear();
    this->MaterialName.clear();

    std::ifstream obj_file(filename);

    if (!obj_file.is_open())
    {
        return false;
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
            this->Vertexs.push_back(vertex);
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
            this->Indexs.push_back(index - 1);
            obj_file >> index;
            this->Indexs.push_back(index - 1);
            obj_file >> index;
            this->Indexs.push_back(index - 1);

            this->MaterialName.push_back(material_name);
        }
        else
        {
            obj_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return true;
}


}

