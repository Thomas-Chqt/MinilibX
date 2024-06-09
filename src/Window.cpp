/*
 * ---------------------------------------------------
 * Window.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 15:05:14
 * ---------------------------------------------------
 */

#include "Window.hpp"
#include "Graphics/Event.hpp"
#include "Graphics/Window.hpp"
#include "Math/Matrix.hpp"
#include "Ptr.hpp"
#include "UtilsCPP/SharedPtr.hpp"
#include "mlx_internal.hpp"

namespace mlx
{

Window::Window(mlx::Ptr& mlx_ptr, int width, int height)
{
    m_graphicAPI = mlx_ptr.platform().newDefaultGraphicAPI();

    m_projectionMatrix = math::mat3x3(
        2/(float)width,                   0, -1,
                     0, -2.0f/(float)height,  1,
                     0,                   0,  1
    );

    utils::SharedPtr<gfx::Window> window = mlx_ptr.platform().newDefaultWindow(width, height);
    
    window->setEventCallBack([this](gfx::Event& gfxEvent)
    {
        for (int i = 0; i < 7; i++)
        {
            if (m_hooks[i])
                m_hooks[i](gfxEvent);
        }
    });
    
    m_graphicAPI->setRenderTarget(window);

    m_graphicPipeline = m_graphicAPI->newGraphicsPipeline("imageDraw_vs", "imageDraw_fs", gfx::GraphicPipeline::BlendingOperation::one_minus_srcA_plus_srcA);
    m_indexBuffer = m_graphicAPI->newIndexBuffer({ 0, 2, 1, 1, 2, 3 });
}

void Window::putImage(mlx::Image& img, int x, int y)
{
    m_graphicAPI->beginFrame(false);

    m_graphicAPI->useGraphicsPipeline(m_graphicPipeline);
    m_graphicAPI->useVertexBuffer(img.makeVertexBuffer(*m_graphicAPI));
    m_graphicAPI->setVertexUniform(m_graphicPipeline->findVertexUniformIndex("u_mpMatrix"), m_projectionMatrix * math::mat3x3(
        1, 0, (float)x,
        0, 1, (float)y,
        0, 0, 1
    ));
    m_graphicAPI->setFragmentTexture(m_graphicPipeline->findFragmentUniformIndex("u_texture"), img.makeTexture(*m_graphicAPI));
    m_graphicAPI->drawIndexedVertices(m_indexBuffer);

    m_graphicAPI->endFrame();
}

void Window::setHook(int x_event, int (*func)(), void* param)
{
    switch (x_event)
    {
    case 2:
        m_hooks[0] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::KeyDownEvent>([&](gfx::KeyDownEvent& gfxEvent){
                reinterpret_cast<int(*)(int, void*)>(func)(convertKeyCode(gfxEvent.keyCode()), param);
            });
        };
        break;

    case 3:
        m_hooks[1] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::KeyUpEvent>([&](gfx::KeyUpEvent& gfxEvent){
                reinterpret_cast<int(*)(int, void*)>(func)(convertKeyCode(gfxEvent.keyCode()), param);
            });
        };
        break;

    case 4:
        m_hooks[2] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::MouseDownEvent>([&](gfx::MouseDownEvent& gfxEvent){
                reinterpret_cast<int(*)(int, int, int, void*)>(func)(convertMouseCode(gfxEvent.mouseCode()), gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
            });
        };
        break;

    case 5:
        m_hooks[3] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::MouseUpEvent>([&](gfx::MouseUpEvent& gfxEvent){
                reinterpret_cast<int(*)(int, int, int, void*)>(func)(convertMouseCode(gfxEvent.mouseCode()), gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
            });
        };
        break;

    case 6:
        m_hooks[4] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::MouseMoveEvent>([&](gfx::MouseMoveEvent& gfxEvent){
                reinterpret_cast<int(*)(int, int, void*)>(func)(gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
            });
        };
        break;

    case 12:
        break;

    case 17:
        m_hooks[6] = [func, param](gfx::Event& gfxEvent){
            gfxEvent.dispatch<gfx::WindowRequestCloseEvent>([&](gfx::WindowRequestCloseEvent& gfxEvent){
                reinterpret_cast<int(*)(void*)>(func)(param);
            });
        };
        break;
    }
}

}