# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 12:32:32
# ---------------------------------------------------

cmake_minimum_required(VERSION 3.23)

option(BUILD_SHARED_LIBS    "Build using shared libraries"        OFF)
option(MLX_IN_PARENT_PROJ   "Add mlx to the parent project"       ON)
option(MLX_INCLUDE_EXEMPLES "Include mlx exemples in the project" OFF)


get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT MLX_IN_PARENT_PROJ OR NOT HAS_PARENT)
    project(MinilibX)
endif()

add_subdirectory("dependencies")

enable_language(CXX)

add_library(mlx)

set_target_properties(mlx PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE MLX_SOURCES "src/*.cpp" "src/*.tpp" "src/*.hpp" "include/*.tpp" "include/*.hpp" "include/*.h")
target_sources(mlx PRIVATE ${MLX_SOURCES})


target_include_directories(mlx PRIVATE "src" PUBLIC "include")


add_subdirectory("Shaders")

if(ENABLE_METAL)
    target_compile_definitions(mlx PRIVATE "MTL_SHADER_LIB=\"${MTL_SHADER_LIB}\"")
    add_dependencies(mlx MLX_MetalShaderLibrary)
endif()

if(ENABLE_OPENGL)
    target_compile_definitions(mlx PRIVATE "OPENGL_SHADER_DIR=\"${OPENGL_SHADER_DIR}\"")
endif()


target_link_libraries(mlx PRIVATE UtilsCPP Graphics)


if(MLX_INCLUDE_EXEMPLES)
    add_subdirectory("exemples")
endif()