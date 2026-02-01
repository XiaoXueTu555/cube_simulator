//
// Created by xiaoxuetu on 2026/1/31.
//

#ifndef RENDERER_VIEW_PORT_H
#define RENDERER_VIEW_PORT_H

#include "Math/vector3d.h"
#include "Math/vector4d.h"
#include <vector>

namespace CS::Renderer
{


/* 视口 */
class Viewport
{

/* 屏幕上的点 */
struct ScreenPoint
{
    int x;
    int y;
    float z;
};

public:
    Viewport(unsigned int width, unsigned height);

    void resize(unsigned int width, unsigned int height);

    unsigned int Width() const;
    unsigned int Height() const;

    /* 清空视口数据 */
    void Clear();

    /* NDC空间 --> 屏幕坐标系
     * 透视除法之后得到的是Vector4d,所以参数传Vector4d
     */
    ScreenPoint NDCToScreen(const Math::Vector4d& ndc_point) const;

    /* 将NDC三角形绘制到视口中
     * 透视除法之后得到的是Vector4d,所以参数传Vector4d
     */
    void DrawTrangle(Math::Vector4d ndc_p1, Math::Vector4d ndc_p2, Math::Vector4d ndc_p3,
                     char glyph, Math::Vector3d color);

    /*
     * 将缓冲区内容输出到控制台
     * is_clear_screen: 是否在输出前清空屏幕（如果不清屏，画面会叠加或滚动）
     */
    void Present(bool is_clear_screen = true);

    /* 字符缓冲区 */
    std::vector<std::vector<char>> char_buffer;
    /* 颜色缓冲区 */
    std::vector<std::vector<Math::Vector3d>> color_buffer;
    /* 深度缓冲区 */
    std::vector<std::vector<float>> z_buffer;

private:
    /* 视口的大小*/
    unsigned int width{};
    unsigned int height{};

    /* 清空z buffer */
    void clear_Zbuffer(float z_inf = 1.0f);

    /* 清空char buffer */
    void clear_char_buffer(char glyph = '.');

    /* 清空color buffer */
    void clear_color_buffer(Math::Vector3d color = Math::Vector3d{0, 0, 0});


};

}

#endif //RENDERER_VIEW_PORT_H
