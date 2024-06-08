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
    set_target_properties(UtilsCPP PROPERTIES FOLDER "dependencies")
    target_link_libraries(mlx PRIVATE UtilsCPP)
endif()
    
if(NOT TARGET Graphics)
    find_program(GIT_PATH git REQUIRED)
    execute_process(COMMAND ${GIT_PATH} submodule update --init Graphics WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(GFX_ENABLE_METAL   ${MLX_ENABLE_METAL})
    set(GFX_ENABLE_OPENGL  ${MLX_ENABLE_OPENGL})
    set(GFX_BUILD_EXEMPLES NO)
    set(GFX_BUILD_IMGUI    NO)
    add_subdirectory(Graphics)
    set_target_properties(Graphics PROPERTIES FOLDER "dependencies")
    target_link_libraries(mlx PRIVATE Graphics)
endif()