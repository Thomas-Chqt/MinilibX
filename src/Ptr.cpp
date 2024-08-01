/*
 * ---------------------------------------------------
 * Ptr.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 15:01:28
 * ---------------------------------------------------
 */

#include "Ptr.hpp"
#include "Graphics/Platform.hpp"

namespace mlx
{

utils::Array<utils::UniquePtr<mlx::Window>> Ptr::s_windows;

Ptr::Ptr()
{
    gfx::Platform::init();

    m_platform = &gfx::Platform::shared();
};

void Ptr::startLoop()
{
    while (1)
    {
        m_platform->pollEvents();

        for (auto& win : s_windows)
            win->drawFrame();
        
        if (m_loopHook)
            m_loopHook();
    }
}

void Ptr::mouseHide()
{
    for (auto& win : s_windows) {
        win->mouseHide();
    }
}

void Ptr::mouseShow()
{
    for (auto& win : s_windows) {
        win->mouseShow();
    }
}

Ptr::~Ptr()
{
    gfx::Platform::terminate();
}

}