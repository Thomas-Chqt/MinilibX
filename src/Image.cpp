/*
 * ---------------------------------------------------
 * Image.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 15:23:39
 * ---------------------------------------------------
 */

#include "Image.hpp"
#include "mlx_internal.hpp"
#include "Vertex.hpp"

namespace mlx
{

Image::Image(int width, int height) : m_width(width), m_height(height)
{
    m_buffer = ::operator new (width * height * BYTES_PER_PIXEL);
}

char* Image::getDataAddr(int *bits_per_pixel, int *size_line, int *endian)
{
    *bits_per_pixel = BITS_PER_PIXEL;
    *size_line = m_width * BYTES_PER_PIXEL;
    *endian = 0;
    return (char*)m_buffer;
}

utils::SharedPtr<gfx::Texture> Image::makeTexture(gfx::GraphicAPI& gfxApi)
{
    utils::SharedPtr<gfx::Texture> newTexture = gfxApi.newTexture(m_width, m_height, gfx::Texture::PixelFormat::ARGB);
    newTexture->setBytes(m_buffer);

    return newTexture;
}

utils::SharedPtr<gfx::VertexBuffer> Image::makeVertexBuffer(gfx::GraphicAPI& gfxApi)
{
    return gfxApi.newVertexBuffer<mlx::Vertex>({
        { {              0,               0 }, { 0, 0 } },
        { {              0, (float)m_height }, { 0, 1 } },
        { { (float)m_width,               0 }, { 1, 0 } },
        { { (float)m_width, (float)m_height }, { 1, 1 } }
    });
}

Image::~Image()
{
    ::operator delete(m_buffer);
}

}