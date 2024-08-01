/*
 * ---------------------------------------------------
 * Image.hpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 14:58:44
 * ---------------------------------------------------
 */

#ifndef IMAGE_HPP
# define IMAGE_HPP

#include "Graphics/GraphicAPI.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "UtilsCPP/SharedPtr.hpp"
namespace mlx
{

class Image
{
public:
    Image(int width, int height);

    char* getDataAddr(int *bits_per_pixel, int *size_line, int *endian);
    utils::SharedPtr<gfx::Texture> makeTexture(gfx::GraphicAPI& gfxApi);
    utils::SharedPtr<gfx::VertexBuffer> makeVertexBuffer(gfx::GraphicAPI& gfxApi);
    void setContent(void* data);

    ~Image();

private:
    int m_width;
    int m_height;
    void* m_buffer;
};

}

#endif // IMAGE_HPP