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
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/GraphicAPI.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Image.hpp"
#include "Math/Matrix.hpp"
#include "UtilsCPP/Array.hpp"
#include "UtilsCPP/Func.hpp"
#include "UtilsCPP/SharedPtr.hpp"
#include "Graphics/Window.hpp"

namespace mlx
{

class Ptr;

class Window
{
private:
    struct PutedTexture
    {
        utils::SharedPtr<gfx::Texture> texture;
        utils::SharedPtr<gfx::VertexBuffer> vtxBuffer;
        math::mat3x3 modelMatrix;
    };
public:
    Window(mlx::Ptr& mlx_ptr, int width, int height);

    void putImage(mlx::Image& img, int x, int y);
    void setHook(int x_event, int (*func)(), void* param);

    void mouse_move(int x, int y);
    void mouse_get_pos(int *x, int *y);

    void drawFrame();

    void mouseHide() { m_window->setCursorVisibility(false); }
    void mouseShow() { m_window->setCursorVisibility(true); }

private:
    utils::SharedPtr<gfx::Window> m_window;
    utils::SharedPtr<gfx::GraphicAPI> m_graphicAPI;
    math::mat3x3 m_projectionMatrix;

    utils::Func<void(gfx::Event&)> m_hooks[7] = {};

    utils::Array<PutedTexture> m_putedTextures;

    utils::SharedPtr<gfx::FrameBuffer> m_frameBuffer;
    utils::SharedPtr<gfx::GraphicPipeline> m_graphicPipeline;
    utils::SharedPtr<gfx::GraphicPipeline> m_graphicPipelineNoBlending;
    utils::SharedPtr<gfx::VertexBuffer> m_vertexBuffer;
    utils::SharedPtr<gfx::IndexBuffer> m_indexBuffer;

    int mousePosX;
    int mousePosY;
};

}

#endif // MLX_WINDOW_HPP