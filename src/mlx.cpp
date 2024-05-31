/*
 * ---------------------------------------------------
 * mlx.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 12:49:04
 * ---------------------------------------------------
 */

#include "mlx/mlx.h"

#include "Graphics/Event.hpp"
#include "Graphics/GraphicAPI.hpp"
#include "Graphics/GraphicPipeline.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Platform.hpp"
#include "Graphics/ShaderLibrary.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/Window.hpp"
#include "Math/Matrix.hpp"
#include "UtilsCPP/Array.hpp"
#include "UtilsCPP/Func.hpp"
#include "UtilsCPP/SharedPtr.hpp"
#include "UtilsCPP/String.hpp"
#include "Vertex.hpp"
#include <new>
#include <GLFW/glfw3.h>

#define BYTES_PER_PIXEL 4
#define BITS_PER_PIXEL BYTES_PER_PIXEL * 8

using namespace utils;

namespace mlx
{

struct MLX_Window
{
    SharedPtr<gfx::GraphicAPI> graphicAPI;
    math::mat3x3 projectionMat;

    bool needDraw = false;

    utils::Func<void(gfx::Event&)> hooks[7] = {};
};

struct mlxPtr
{
    gfx::Platform& platform;
    utils::Array<MLX_Window*> windows;
    utils::Func<void()> loopHook;
};

struct MLX_Image
{
    int width;
    int height;
    void* buffer;
};

int convertKeyCode(int code)
{
    switch (code)
    {
        case  0: return 53;
        case 32: return 49;
        case 49: return 18;
        case 50: return 19;
        case 51: return 20;
        case 52: return 21;
        case 53: return 23;
        case  7: return 123;
        case  6: return 124;
        case  9: return 126;
        case  8: return 125;
        case 82: return 15;
        default: return code;
    }
}
int convertMouseCode(int code)
{
    switch (code)
    {
        case 0:  return 1;
        case 1:  return 2;
        default: return code;
    }
}

void* mlx_init()
{
    try
    {
        gfx::Platform::init();
        gfx::ShaderLibrary::init();

        #ifdef USING_METAL
            gfx::ShaderLibrary::shared().setMetalShaderLibPath(MTL_SHADER_LIB);
        #endif

        gfx::ShaderLibrary::shared().registerShader("imageDraw_vrtx"
            #ifdef USING_METAL
            , "imageDraw_vrtx"
            #endif
            #ifdef USING_OPENGL
            , utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw_vrtx.glsl")
            #endif
        );
        gfx::ShaderLibrary::shared().registerShader("imageDraw_frag"
            #ifdef USING_METAL
            , "imageDraw_frag"
            #endif
            #ifdef USING_OPENGL
            , utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw_frag.glsl")
            #endif
        );

        return new mlxPtr { .platform = gfx::Platform::shared() };
    }
    catch (...) { return nullptr; }
}

void* mlx_new_window(void* mlx_ptr, int width, int height, char* title)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);

        MLX_Window* mlxWindow = new MLX_Window;

        mlxWindow->graphicAPI = mlx.platform.newDefaultGraphicAPI();
        mlxWindow->projectionMat = math::mat3x3(
            2/(float)width,                   0, -1,
                         0, -2.0f/(float)height,  1,
                         0,                   0,  1);

        utils::SharedPtr<gfx::Window> window = mlx.platform.newDefaultWindow(width, height);

        window->setEventCallBack([mlxWindow](gfx::Event& gfxEvent){
            for (int i = 0; i < 7; i++)
            {
                if (mlxWindow->hooks[i])
                    mlxWindow->hooks[i](gfxEvent);
            }
        });

        mlx.windows.append(mlxWindow);

        mlxWindow->graphicAPI->setRenderTarget(window);
        mlxWindow->graphicAPI->beginFrame();

        return mlxWindow;
    }
    catch (...) { return nullptr; }
}

int mlx_destroy_window(void* mlx_ptr, void* win_ptr)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);
        MLX_Window* mlxWin = static_cast<MLX_Window*>(win_ptr);
        delete mlxWin;
        return 0;
    }
    catch (...) { return 1; }
}

void* mlx_new_image(void* mlx_ptr, int width, int height)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);
        return new MLX_Image { width, height, ::operator new (width * height * BYTES_PER_PIXEL) };
    }
    catch (...) { return nullptr; }
}

int mlx_destroy_image(void* mlx_ptr, void* img_ptr)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);
        MLX_Image* mlxImg = static_cast<MLX_Image*>(img_ptr);

        ::operator delete(mlxImg->buffer);
        delete mlxImg;
        return 0;
    }
    catch (...) { return 1; }
}

char* mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian)
{
    *bits_per_pixel = BITS_PER_PIXEL;
    *size_line = ((MLX_Image*)img_ptr)->width * BYTES_PER_PIXEL;
    *endian = 0;
    return (char*)((MLX_Image*)img_ptr)->buffer;
}

int mlx_put_image_to_window(void* mlx_ptr, void* win_ptr, void* img_ptr, int x, int y)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);
        MLX_Window& mlxWindow = *static_cast<MLX_Window*>(win_ptr);
        MLX_Image& mlxImg = *static_cast<MLX_Image*>(img_ptr);

        SharedPtr<gfx::Texture> texture = mlxWindow.graphicAPI->newTexture(mlxImg.width, mlxImg.height, gfx::Texture::PixelFormat::ARGB);
        texture->setBytes(mlxImg.buffer);

        SharedPtr<gfx::GraphicPipeline> graphicPipeline = mlxWindow.graphicAPI->newGraphicsPipeline("imageDraw_vrtx", "imageDraw_frag", gfx::GraphicPipeline::BlendingOperation::one_minus_srcA_plus_srcA);

        SharedPtr<gfx::VertexBuffer> vertexBuffer = mlxWindow.graphicAPI->newVertexBuffer<Vertex>({
            { .pos = {                   0,                    0 }, .uv = { 0, 0 } },
            { .pos = {                   0, (float)mlxImg.height }, .uv = { 0, 1 } },
            { .pos = { (float)mlxImg.width,                    0 }, .uv = { 1, 0 } },
            { .pos = { (float)mlxImg.width, (float)mlxImg.height }, .uv = { 1, 1 } }
        });

        SharedPtr<gfx::IndexBuffer> indexBuffer = mlxWindow.graphicAPI->newIndexBuffer({ 
            0, 2, 1,
            1, 2, 3
        });

        math::mat3x3 modelMatrix = {
            1, 0, (float)x,
            0, 1, (float)y,
            0, 0, 1
        };
        
        mlxWindow.graphicAPI->useGraphicsPipeline(graphicPipeline);
        mlxWindow.graphicAPI->useVertexBuffer(vertexBuffer);

        mlxWindow.graphicAPI->setVertexUniform(graphicPipeline->findVertexUniformIndex("u_mpMatrix"),    mlxWindow.projectionMat * modelMatrix);
        mlxWindow.graphicAPI->setFragmentTexture(graphicPipeline->findFragmentUniformIndex("u_texture"), texture);

        mlxWindow.graphicAPI->drawIndexedVertices(indexBuffer);

        mlxWindow.needDraw = true;

        return 0;
    }
    catch (...) { return 1; }
}

int mlx_hook(void* win_ptr, int x_event, int x_mask, int (*func)(), void* param)
{
    try
    {
        MLX_Window& mlxWindow = *static_cast<MLX_Window*>(win_ptr);

        switch (x_event)
        {
        case 2:
            mlxWindow.hooks[0] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::KeyDownEvent>([&](gfx::KeyDownEvent& gfxEvent){
                    reinterpret_cast<int(*)(int, void*)>(func)(convertKeyCode(gfxEvent.keyCode()), param);
                });
            };
            break;

        case 3:
            mlxWindow.hooks[1] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::KeyUpEvent>([&](gfx::KeyUpEvent& gfxEvent){
                    reinterpret_cast<int(*)(int, void*)>(func)(convertKeyCode(gfxEvent.keyCode()), param);
                });
            };
            break;

        case 4:
            mlxWindow.hooks[2] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::MouseDownEvent>([&](gfx::MouseDownEvent& gfxEvent){
                    reinterpret_cast<int(*)(int, int, int, void*)>(func)(convertMouseCode(gfxEvent.mouseCode()), gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
                });
            };
            break;

        case 5:
            mlxWindow.hooks[3] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::MouseUpEvent>([&](gfx::MouseUpEvent& gfxEvent){
                    reinterpret_cast<int(*)(int, int, int, void*)>(func)(convertMouseCode(gfxEvent.mouseCode()), gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
                });
            };
            break;

        case 6:
            mlxWindow.hooks[4] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::MouseMoveEvent>([&](gfx::MouseMoveEvent& gfxEvent){
                    reinterpret_cast<int(*)(int, int, void*)>(func)(gfxEvent.mousePosX(), gfxEvent.mousePosY(), param);
                });
            };
            break;

        case 12:
            break;

        case 17:
            mlxWindow.hooks[6] = [func, param](gfx::Event& gfxEvent){
                gfxEvent.dispatch<gfx::WindowRequestCloseEvent>([&](gfx::WindowRequestCloseEvent& gfxEvent){
                    reinterpret_cast<int(*)(void*)>(func)(param);
                });
            };
            break;
        }
        
        
        return 0;
    } catch (...) { return 1; }
}

int mlx_loop_hook(void* mlx_ptr, int (*func)(), void* param)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);
        mlx.loopHook = [func, param](){ reinterpret_cast<int(*)(void*)>(func)(param); };
        return 0;
    }
    catch (...) { return 1; }
}

int mlx_loop(void* mlx_ptr)
{
    try
    {
        mlxPtr& mlx = *static_cast<mlxPtr*>(mlx_ptr);

        while (1)
        {
            mlx.platform.pollEvents();
            for (auto& win : mlx.windows)
            {
                if (win->needDraw)
                {
                    win->graphicAPI->endFrame();
                    win->graphicAPI->beginFrame();
                    win->needDraw = false;
                }
            }
            if (mlx.loopHook)
                mlx.loopHook();
        }
        return 0;
    }
    catch(...) { return 1; }
}

}
