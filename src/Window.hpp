/*
 * ---------------------------------------------------
 * Window.hpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 14:55:54
 * ---------------------------------------------------
 */

#ifndef MLX_WINDOW_HPP
# define MLX_WINDOW_HPP

#include "Graphics/Event.hpp"
#include "Graphics/GraphicAPI.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Image.hpp"
#include "Math/Matrix.hpp"
#include "UtilsCPP/Func.hpp"
#include "UtilsCPP/SharedPtr.hpp"

namespace mlx
{

class Ptr;

class Window
{
public:
    Window(mlx::Ptr& mlx_ptr, int width, int height);

    void putImage(mlx::Image& img, int x, int y);
    void setHook(int x_event, int (*func)(), void* param);

    void drawFrame();

private:
    utils::SharedPtr<gfx::GraphicAPI> m_graphicAPI;
    math::mat3x3 m_projectionMatrix;
    utils::Func<void(gfx::Event&)> m_hooks[7] = {};
    utils::SharedPtr<gfx::GraphicPipeline> m_graphicPipeline;
    utils::SharedPtr<gfx::IndexBuffer> m_indexBuffer;
};

}

#endif // MLX_WINDOW_HPP