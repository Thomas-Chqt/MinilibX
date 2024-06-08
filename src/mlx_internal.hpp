/*
 * ---------------------------------------------------
 * mlx_internal.hpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 13:12:40
 * ---------------------------------------------------
 */

#ifndef MLX_INTERNAL_HPP
# define MLX_INTERNAL_HPP

#define BYTES_PER_PIXEL 4
#define BITS_PER_PIXEL BYTES_PER_PIXEL * 8

namespace mlx
{

int convertKeyCode(int code);
int convertMouseCode(int code);

}

#endif // MLX_INTERNAL_HPP