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
#include "Graphics/ShaderLibrary.hpp"

namespace mlx
{

Ptr::Ptr()
{
    gfx::Platform::init();
    gfx::ShaderLibrary::init();

    m_platform = &gfx::Platform::shared();

    #ifdef GFX_METAL_ENABLED
        gfx::ShaderLibrary::shared().setMetalShaderLibPath(MTL_SHADER_LIB);
    #endif

    gfx::ShaderLibrary::shared().registerShader("imageDraw_vs"
        #ifdef GFX_METAL_ENABLED
        , "imageDraw_vs"
        #endif
        #ifdef GFX_OPENGL_ENABLED
        , utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.vs")
        #endif
    );
    gfx::ShaderLibrary::shared().registerShader("imageDraw_fs"
        #ifdef GFX_METAL_ENABLED
        , "imageDraw_fs"
        #endif
        #ifdef GFX_OPENGL_ENABLED
        , utils::String::contentOfFile(OPENGL_SHADER_DIR"/imageDraw.fs")
        #endif
    );
};

void Ptr::startLoop()
{
    while (1)
    {
        m_platform->pollEvents();
        
        if (m_loopHook)
            m_loopHook();
    }
}

Ptr::~Ptr()
{
    gfx::ShaderLibrary::terminated();
    gfx::Platform::terminate();
}

}