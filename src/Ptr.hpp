/*
 * ---------------------------------------------------
 * Ptr.hpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 14:57:11
 * ---------------------------------------------------
 */

#ifndef PTR_HPP
# define PTR_HPP

#include "Graphics/Platform.hpp"
#include "UtilsCPP/Array.hpp"
#include "UtilsCPP/Func.hpp"
#include "UtilsCPP/UniquePtr.hpp"
#include "Window.hpp"

namespace mlx
{

class Ptr
{
public:
    Ptr();

    inline gfx::Platform& platform() { return *m_platform; }
    inline void addWindow(utils::UniquePtr<mlx::Window>&& win) { m_windows.append(std::move(win)); }
    inline void removeWindow(mlx::Window* win) { m_windows.remove(m_windows.find(win)); }
    inline void setLoopHook(const utils::Func<void()>& func) { m_loopHook = func; }
    void startLoop();

    ~Ptr();

private:
    gfx::Platform* m_platform;
    utils::Array<utils::UniquePtr<mlx::Window>> m_windows;
    utils::Func<void()> m_loopHook;
};

}

#endif // PTR_HPP