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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

int mlx_mouse_hide()
{
    Ptr::mouseHide();
    return 0;
}

int mlx_mouse_show()
{
    Ptr::mouseShow();
    return 0;
}

int mlx_mouse_move(void *win_ptr, int x, int y)
{
    try
    {
        mlx::Window& mlxWindow = *static_cast<mlx::Window*>(win_ptr);
        mlxWindow.mouse_move(x, y);
        return 0;
    } 
    catch (...) { return 1; }
}

int mlx_mouse_get_pos(void *win_ptr, int *x, int *y)
{
    try
    {
        mlx::Window& mlxWindow = *static_cast<mlx::Window*>(win_ptr);
        mlxWindow.mouse_get_pos(x, y);
        return 0;
    } 
    catch (...) { return 1; }
}

void* mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height)
{
    try
    {
        (void)mlx_ptr;
        *width = 100;
        *height = 100;
        stbi_uc* imgBytes = stbi_load(filename, width, height, nullptr, STBI_rgb_alpha);
        if (imgBytes == nullptr)
            return nullptr;
        mlx::Image* mlxImg = new mlx::Image(*width, *height);
        mlxImg->setContent(imgBytes);
        stbi_image_free(imgBytes);
        return mlxImg;
    }
    catch (...) { return nullptr; }
}

void* mlx_png_file_to_image(void *mlx_ptr, char *file, int *width, int *height)
{
    return mlx_xpm_file_to_image(mlx_ptr, file, width, height);
}

}
