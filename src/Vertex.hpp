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

template<>
inline utils::Array<gfx::VertexBuffer::LayoutElement> gfx::VertexBuffer::getLayout<mlx::Vertex>()
{
    return {
        { 2, Type::FLOAT, false, sizeof(mlx::Vertex), (void*)0 },
        { 2, Type::FLOAT, false, sizeof(mlx::Vertex), (void*)offsetof(mlx::Vertex, uv) },
    };
}

#endif // __METAL_VERSION__