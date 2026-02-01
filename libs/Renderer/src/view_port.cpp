//
// Created by xiaoxuetu on 2026/1/31.
//

#include "Renderer/view_port.h"
#include <cmath>
#include <algorithm>
#include <iostream>


namespace CS::Renderer
{
Viewport::Viewport(unsigned int width, unsigned height)
{
    this->resize(width, height);
}

void Viewport::resize(const unsigned int width, const unsigned int height)
{
    this->width = width;
    this->height = height;

    this->char_buffer.resize(height);
    for (auto& raw : this->char_buffer)
    {
        raw.resize(width);
    }

    this->color_buffer.resize(height);
    for (auto& raw : this->color_buffer)
    {
        raw.resize(width);
    }

    this->z_buffer.resize(height);
    for (auto& raw : this->z_buffer)
    {
        raw.resize(width);
    }

    this->Clear();
}

unsigned int Viewport::Width() const
{
    return this->width;
}

unsigned int Viewport::Height() const
{
    return this->height;
}

void Viewport::Clear()
{
    this->clear_Zbuffer();
    this->clear_char_buffer();
    this->clear_color_buffer();
}

Viewport::ScreenPoint Viewport::NDCToScreen(const Math::Vector4d& ndc_point) const
{
    Viewport::ScreenPoint screen_point
    {
        std::lroundf(width  * (ndc_point.x + 1) * 0.5f),
        std::lroundf(height * (1 - ndc_point.y) * 0.5f),
        ndc_point.z
    };

    return screen_point;
}

void Viewport::DrawTrangle(Math::Vector4d ndc_p1, Math::Vector4d ndc_p2, Math::Vector4d ndc_p3, char glyph,
                           Math::Vector3d color)
{
    /* NDC -> 屏幕坐标转换 */
    ScreenPoint p0 = NDCToScreen(ndc_p1);
    ScreenPoint p1 = NDCToScreen(ndc_p2);
    ScreenPoint p2 = NDCToScreen(ndc_p3);

    /* 计算包围盒 */
    int min_x = std::min({p0.x, p1.x, p2.x});
    int max_x = std::max({p0.x, p1.x, p2.x});
    int min_y = std::min({p0.y, p1.y, p2.y});
    int max_y = std::max({p0.y, p1.y, p2.y});

    /* 限制包围盒在屏幕范围内 (防止越界崩溃) */
    min_x = std::max(0, min_x);
    min_y = std::max(0, min_y);
    max_x = std::min(static_cast<int>(this->width) - 1, max_x);
    max_y = std::min(static_cast<int>(this->height) - 1, max_y);

    /* 提取顶点数据，减少循环内访问 */
    int x0 = p0.x, y0 = p0.y;
    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;
    float z0 = p0.z, z1 = p1.z, z2 = p2.z;

    /* 光栅化：遍历包围盒内的所有像素 */
    for (int y = min_y; y <= max_y; y++)
    {
        for (int x = min_x; x <= max_x; x++)
        {
            /* 重心坐标计算 (Edge Function 叉积法) */
            // det = 总面积的2倍
            int det = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);

            if (det == 0) continue; // 避免除以零（三角形退化）

            // 计算当前像素点 相对于三个顶点的权重
            int w0 = (y1 - y2) * (x - x2) + (x2 - x1) * (y - y2);
            int w1 = (y2 - y0) * (x - x2) + (x0 - x2) * (y - y2);
            int w2 = det - w0 - w1;

            /* 判断点是否在三角形内 */
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                /* 深度插值 */
                float u = static_cast<float>(w0) / det;
                float v = static_cast<float>(w1) / det;
                float w = static_cast<float>(w2) / det;
                float pixel_depth = u * z0 + v * z1 + w * z2;

                /* 深度测试 */
                // 如果当前像素深度比记录的深度小（更近），则绘制。
                if (pixel_depth < this->z_buffer[y][x])
                {
                    this->z_buffer[y][x] = pixel_depth;
                    this->char_buffer[y][x] = glyph;
                    this->color_buffer[y][x] = color;
                }
            }
        }
    }
}

void Viewport::Present(bool is_clear_screen)
{
    // // ANSI 转义序列：将光标移动到左上角 (0,0)
    // // \033 或 \x1b 是 ESC 字符
    // std::cout << "\033[H";
    // // 如果需要清屏（可选，通常如果每次填满屏幕，覆盖即可，不需要清屏以防闪烁）
    // // std::cout << "\033[2J";
    // for (unsigned int y = 0; y < this->height; ++y)
    // {
    //     for (unsigned int x = 0; x < this->width; ++x)
    //     {
    //         // 获取颜色 (注意：假设您的 Vector3d 是 0.0~1.0 的 float)
    //         // 需要乘以 255 转成整数 RGB
    //         Math::Vector3d col = this->color_buffer[y][x];
    //         int r = static_cast<int>(col.x * 255);
    //         int g = static_cast<int>(col.y * 255);
    //         int b = static_cast<int>(col.z * 255);
    //         char glyph = this->char_buffer[y][x];
    //         // ANSI 颜色设置格式：\033[38;2;R;G;Bm
    //         // 这意味着：设置前景色为 RGB(r,g,b)
    //         std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    //         // 打印字符
    //         std::cout << glyph;
    //     }
    //     // 每一行结束输出一个换行符
    //     std::cout << "\n";
    // }
    // // 恢复默认颜色（可选，防止下次控制台输出文字也是彩色的）
    // std::cout << "\033[0m";

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            std::cout << this->char_buffer[i][j];
        }
        std::cout << std::endl;
    }
}

void Viewport::clear_Zbuffer(float z_inf)
{
    /* 假设一开始的深度都是无穷远 */
    for (auto& raw : this->z_buffer)
    {
        for (auto& col : raw)
        {
            col = z_inf;
        }
    }
}

void Viewport::clear_char_buffer(char glyph)
{
    for (auto& raw : this->char_buffer)
    {
        for (auto& col : raw)
        {
            col = glyph;
        }
    }
}

void Viewport::clear_color_buffer(Math::Vector3d color)
{
    for (auto& raw : this->color_buffer)
    {
        for (auto& col : raw)
        {
            col = color;
        }
    }
}

}
