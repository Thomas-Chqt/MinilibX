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
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/GraphicPipeline.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/Window.hpp"
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
    utils::SharedPtr<gfx::Window> window = mlx_ptr.platform().newDefaultWindow(width, height);
    window->setEventCallBack([this](gfx::Event& gfxEvent)
    {
        for (int i = 0; i < 7; i++)
        {
            if (m_hooks[i])
                m_hooks[i](gfxEvent);
        }
    });
    
    m_graphicAPI = mlx_ptr.platform().newDefaultGraphicAPI(window);
    #ifdef MLX_METAL_ENABLED
        m_graphicAPI->initMetalShaderLib(MTL_SHADER_LIB);
    #endif

    m_projectionMatrix = math::mat3x3(
        2/(float)width,                   0, -1,
                     0, -2.0f/(float)height,  1,
                     0,                   0,  1
    );

    gfx::FrameBuffer::Descriptor fBuffDesc;
    fBuffDesc.width = width;
    fBuffDesc.height = height;
    fBuffDesc.pixelFormat = gfx::PixelFormat::ARGB;
    m_frameBuffer = m_graphicAPI->newFrameBuffer(fBuffDesc);
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
    gfx::GraphicPipeline::Descriptor gfxPipeDesc;

    gfxPipeDesc.metalVSFunction = "imageDraw_vs";
    gfxPipeDesc.metalFSFunction = "imageDraw_fs";
    gfxPipeDesc.openglVSCode = utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.vs");
    gfxPipeDesc.openglFSCode = utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.fs");

    gfxPipeDesc.pixelFormat = gfx::PixelFormat::ARGB;
    gfxPipeDesc.blendOperation = gfx::BlendOperation::one_minus_srcA_plus_srcA;
    utils::SharedPtr<gfx::GraphicPipeline> offscreenPipeline = m_graphicAPI->newGraphicsPipeline(gfxPipeDesc);

    gfxPipeDesc.pixelFormat = gfx::PixelFormat::RGBA;
    gfxPipeDesc.blendOperation = gfx::BlendOperation::srcA_plus_1_minus_srcA;
    utils::SharedPtr<gfx::GraphicPipeline> onscreenPipeline = m_graphicAPI->newGraphicsPipeline(gfxPipeDesc);

    utils::SharedPtr<gfx::VertexBuffer> onscreenVertexBuffer = m_graphicAPI->newVertexBuffer(utils::Array<Vertex>({
        {{-1,  1}, {0, 0}},
        {{-1, -1}, {0, 1}},
        {{ 1,  1}, {1, 0}},
        {{ 1, -1}, {1, 1}},
    }));

    utils::SharedPtr<gfx::IndexBuffer> indexBuffer = m_graphicAPI->newIndexBuffer({0, 2, 1, 1, 2, 3});

    m_graphicAPI->setLoadAction(gfx::LoadAction::load);
    m_graphicAPI->setRenderTarget(m_frameBuffer);
    m_graphicAPI->beginFrame();

    m_graphicAPI->useGraphicsPipeline(offscreenPipeline);
    for (auto& tex : m_putedTextures)
    {
        m_graphicAPI->useVertexBuffer(tex.vtxBuffer);
        m_graphicAPI->setVertexUniform(offscreenPipeline->findVertexUniformIndex("u_mpMatrix"), m_projectionMatrix * tex.modelMatrix);
        m_graphicAPI->setFragmentTexture(offscreenPipeline->findFragmentUniformIndex("u_texture"), tex.texture);
        m_graphicAPI->drawIndexedVertices(indexBuffer);
    }
    m_putedTextures.clear();

    m_graphicAPI->setLoadAction(gfx::LoadAction::clear);
    m_graphicAPI->setRenderTarget(m_graphicAPI->screenFrameBuffer());
    m_graphicAPI->nextRenderPass();

    m_graphicAPI->useGraphicsPipeline(onscreenPipeline);
    m_graphicAPI->useVertexBuffer(onscreenVertexBuffer);
    m_graphicAPI->setVertexUniform(onscreenPipeline->findVertexUniformIndex("u_mpMatrix"), math::mat3x3(1));
    m_graphicAPI->setFragmentTexture(onscreenPipeline->findFragmentUniformIndex("u_texture"), m_frameBuffer);
    m_graphicAPI->drawIndexedVertices(indexBuffer);

    m_graphicAPI->endFrame();
}

}