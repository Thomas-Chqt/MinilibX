/*
 * ---------------------------------------------------
 * mlx.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/05/25 12:49:04
 * ---------------------------------------------------
 */

#include "mlx/mlx.h"
#include "Image.hpp"
#include "Ptr.hpp"
#include "Window.hpp"

namespace mlx
{

void* mlx_init()
{
    try
    {
        return new mlx::Ptr();
    }
    catch (...) { return nullptr; }
}

void* mlx_new_window(void* mlx_ptr, int width, int height, char* title)
{
    try
    {
        mlx::Ptr& mlxPtr = *static_cast<mlx::Ptr*>(mlx_ptr);
        mlx::Window* newWindow = new mlx::Window(mlxPtr, width, height);
        mlxPtr.addWindow(utils::UniquePtr<mlx::Window>(newWindow));
        return newWindow;
    }
    catch (...) { return nullptr; }
}

int mlx_destroy_window(void* mlx_ptr, void* win_ptr)
{
    try
    {
        mlx::Ptr& mlxPtr = *static_cast<mlx::Ptr*>(mlx_ptr);
        mlxPtr.removeWindow((mlx::Window*)win_ptr);
        return 0;
    }
    catch (...) { return 1; }
}

void* mlx_new_image(void* mlx_ptr, int width, int height)
{
    try
    {
        (void)mlx_ptr;
        return new mlx::Image(width, height);
    }
    catch (...) { return nullptr; }
}

int mlx_destroy_image(void* mlx_ptr, void* img_ptr)
{
    try
    {
        (void)mlx_ptr;
        delete static_cast<mlx::Image*>(img_ptr);
        return 0;
    }
    catch (...) { return 1; }
}

char* mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian)
{
    mlx::Image& mlxImg = *static_cast<mlx::Image*>(img_ptr);
    return mlxImg.getDataAddr(bits_per_pixel, size_line, endian);
}

int mlx_put_image_to_window(void* mlx_ptr, void* win_ptr, void* img_ptr, int x, int y)
{
    try
    {
        (void)mlx_ptr;
        mlx::Window& mlxWindow = *static_cast<mlx::Window*>(win_ptr);
        mlx::Image& mlxImg = *static_cast<mlx::Image*>(img_ptr);
        mlxWindow.putImage(mlxImg, x, y);
        return 0;
    }
    catch (...) { return 1; }
}

int mlx_hook(void* win_ptr, int x_event, int x_mask, int (*func)(), void* param)
{
    try
    {
        (void)x_mask;
        mlx::Window& mlxWindow = *static_cast<mlx::Window*>(win_ptr);
        mlxWindow.setHook(x_event, func, param);
        return 0;
    } catch (...) { return 1; }
}

int mlx_loop_hook(void* mlx_ptr, int (*func)(), void* param)
{
    try
    {
        mlx::Ptr& mlxPtr = *static_cast<mlx::Ptr*>(mlx_ptr);
        mlxPtr.setLoopHook([func, param](){ reinterpret_cast<int(*)(void*)>(func)(param); });
        return 0;
    }
    catch (...) { return 1; }
}

int mlx_loop(void* mlx_ptr)
{
    try
    {
        mlx::Ptr& mlxPtr = *static_cast<mlx::Ptr*>(mlx_ptr);
        mlxPtr.startLoop();
        return 0;
    }
    catch(...) { return 1; }
}

}
