/*
 * ---------------------------------------------------
 * Vertex.hpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 19:38:49
 * ---------------------------------------------------
 */

#include "../dependencies/Graphics/dependencies/Math/include/Math/Vector.hpp"

namespace mlx
{

struct Vertex
{
    math::vec2f pos;
    math::vec2f uv;
};

}

#ifndef __METAL_VERSION__

#include "Graphics/VertexBuffer.hpp"

namespace gfx
{

template<>
class VertexBuffer::Layout<mlx::Vertex> : public VertexBuffer::LayoutBase
{
public:
    #ifdef GFX_OPENGL_ENABLED
    Layout()
    {
        m_elements.append({ 2, GL_FLOAT, GL_FALSE, sizeof(mlx::Vertex), (void*)0  });
        m_elements.append({ 2, GL_FLOAT, GL_FALSE, sizeof(mlx::Vertex), (void*)offsetof(mlx::Vertex, uv) });
    }
    inline const utils::Array<Element>& getElements() const override { return m_elements; };
    #endif
    inline utils::uint64 getSize() const override { return sizeof(mlx::Vertex); };

#ifdef GFX_OPENGL_ENABLED
private:
    utils::Array<Element> m_elements;
#endif
};

}

#endif // __METAL_VERSION__