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