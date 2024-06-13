# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 14:16:35
# ---------------------------------------------------

if(NOT TARGET UtilsCPP)
    find_program(GIT_PATH git REQUIRED)
    execute_process(COMMAND ${GIT_PATH} submodule update --init UtilsCPP WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(UtilsCPP)
endif()
set_target_properties(UtilsCPP PROPERTIES FOLDER "dependencies")
target_link_libraries(mlx PRIVATE UtilsCPP)
    
if(NOT TARGET Graphics)
    find_program(GIT_PATH git REQUIRED)
    execute_process(COMMAND ${GIT_PATH} submodule update --init Graphics WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    if(MLX_USING_METAL)
        set(GFX_ENABLE_METAL  ON)
        set(GFX_ENABLE_OPENGL OFF)
    else()
        set(GFX_ENABLE_METAL  OFF)
        set(GFX_ENABLE_OPENGL ON)
    endif()
    set(GFX_BUILD_EXEMPLES OFF)
    set(GFX_BUILD_IMGUI    OFF)
    add_subdirectory(Graphics)
endif()
set_target_properties(Graphics PROPERTIES FOLDER "dependencies")
target_link_libraries(mlx PRIVATE Graphics)