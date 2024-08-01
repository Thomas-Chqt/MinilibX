/*
 * ---------------------------------------------------
 * Window.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 15:05:14
 * ---------------------------------------------------
 */

#include "Window.hpp"
#include "Graphics/Enums.hpp"
#include "Graphics/Event.hpp"
#include "Graphics/GraphicPipeline.hpp"
#include "Graphics/Texture.hpp"
#include "Math/Matrix.hpp"
#include "Ptr.hpp"
#include "UtilsCPP/Array.hpp"
#include "UtilsCPP/SharedPtr.hpp"
#include "mlx_internal.hpp"
#include "Vertex.hpp"

namespace mlx
{

Window::Window(mlx::Ptr& mlx_ptr, int width, int height)
{
    m_window = mlx_ptr.platform().newDefaultWindow(width, height);
    m_window->setEventCallBack([this](gfx::Event& gfxEvent)
    {
        for (int i = 0; i < 7; i++)
        {
            if (m_hooks[i])
                m_hooks[i](gfxEvent);
        }
        gfxEvent.dispatch<gfx::MouseMoveEvent>([&](gfx::MouseMoveEvent& event){
            mousePosX = event.mousePosX();
            mousePosY = event.mousePosY();
        });
    });
    
    m_graphicAPI = mlx_ptr.platform().newDefaultGraphicAPI(m_window);
    #ifdef MLX_USING_METAL
        m_graphicAPI->initMetalShaderLib(MTL_SHADER_LIB);
    #endif

    m_projectionMatrix = math::mat3x3(
        2/(float)width,                   0, -1,
                     0, -2.0f/(float)height,  1,
                     0,                   0,  1
    );

    gfx::Texture::Descriptor colorTextureDescriptor;
    colorTextureDescriptor.width = width;
    colorTextureDescriptor.height = height;
    colorTextureDescriptor.pixelFormat = gfx::PixelFormat::BGRA;
    m_frameBuffer = m_graphicAPI->newFrameBuffer(
        m_graphicAPI->newTexture(colorTextureDescriptor)
    );

    gfx::GraphicPipeline::Descriptor gfxPipeDesc;

    gfxPipeDesc.metalVSFunction = "imageDraw_vs";
    gfxPipeDesc.metalFSFunction = "imageDraw_fs";
    #ifdef MLX_USING_OPENGL
        gfxPipeDesc.openglVSCode = utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.vs");
        gfxPipeDesc.openglFSCode = utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.fs");
    #endif
    gfxPipeDesc.pixelFormat = gfx::PixelFormat::BGRA;
    gfxPipeDesc.blendOperation = gfx::BlendOperation::one_minus_srcA_plus_srcA;
    m_graphicPipeline = m_graphicAPI->newGraphicsPipeline(gfxPipeDesc);

    gfxPipeDesc.blendOperation = gfx::BlendOperation::blendingOff;
    m_graphicPipelineNoBlending = m_graphicAPI->newGraphicsPipeline(gfxPipeDesc);

    #if defined (MLX_USING_OPENGL)
        m_vertexBuffer = m_graphicAPI->newVertexBuffer(utils::Array<Vertex>({
            {{-1,  1}, {0, 1}},
            {{-1, -1}, {0, 0}},
            {{ 1,  1}, {1, 1}},
            {{ 1, -1}, {1, 0}},
        }));
    #else
        m_vertexBuffer = m_graphicAPI->newVertexBuffer(utils::Array<Vertex>({
            {{-1,  1}, {0, 0}},
            {{-1, -1}, {0, 1}},
            {{ 1,  1}, {1, 0}},
            {{ 1, -1}, {1, 1}},
        }));
    #endif

    m_indexBuffer = m_graphicAPI->newIndexBuffer({0, 2, 1, 1, 2, 3});
}

void Window::putImage(mlx::Image& img, int x, int y)
{
    m_putedTextures.append({
        img.makeTexture(*m_graphicAPI),
        img.makeVertexBuffer(*m_graphicAPI),
        math::mat3x3(
            1, 0, (float)x,
            0, 1, (float)y,
            0, 0, 1
        )
    });
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
            gfxEvent.dispatch<gfx::ScrollEvent>([&](gfx::ScrollEvent& gfxEvent){
                int mouseCode;
                if (gfxEvent.offsetY() > 0)
                    mouseCode = 5;
                else
                    mouseCode = 4;
                reinterpret_cast<int(*)(int, int, int, void*)>(func)(mouseCode, gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
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

void Window::drawFrame()
{
    m_graphicAPI->beginFrame();

    static bool shouldClearFrameBuff = true;
    m_graphicAPI->setLoadAction(shouldClearFrameBuff ? gfx::LoadAction::clear : gfx::LoadAction::load);
    shouldClearFrameBuff = false;
    
    m_graphicAPI->beginOffScreenRenderPass(m_frameBuffer);

    m_graphicAPI->useGraphicsPipeline(m_graphicPipeline);
    for (auto& tex : m_putedTextures)
    {
        m_graphicAPI->useVertexBuffer(tex.vtxBuffer);
        m_graphicAPI->setVertexUniform(m_graphicPipeline->findVertexUniformIndex("u_mpMatrix"), m_projectionMatrix * tex.modelMatrix);
        m_graphicAPI->setFragmentTexture(m_graphicPipeline->findFragmentUniformIndex("u_texture"), tex.texture);
        m_graphicAPI->drawIndexedVertices(m_indexBuffer);
    }
    m_putedTextures.clear();

    m_graphicAPI->endOffScreeRenderPass();

    m_graphicAPI->setLoadAction(gfx::LoadAction::clear);
    m_graphicAPI->beginOnScreenRenderPass();

    m_graphicAPI->useGraphicsPipeline(m_graphicPipelineNoBlending);
    m_graphicAPI->useVertexBuffer(m_vertexBuffer);
    m_graphicAPI->setVertexUniform(m_graphicPipelineNoBlending->findVertexUniformIndex("u_mpMatrix"), math::mat3x3(1));
    m_graphicAPI->setFragmentTexture(m_graphicPipelineNoBlending->findFragmentUniformIndex("u_texture"), m_frameBuffer->colorTexture());
    m_graphicAPI->drawIndexedVertices(m_indexBuffer);

    m_graphicAPI->endOnScreenRenderPass();
    m_graphicAPI->endFrame();
}

void Window::mouse_move(int x, int y)
{
    m_window->setCursorPos(x, y);
    mousePosX = x;
    mousePosY = y;
}

void Window::mouse_get_pos(int *x, int *y)
{
    *x = mousePosX;
    *y = mousePosY;
}

}