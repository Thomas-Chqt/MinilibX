# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 12:32:32
# ---------------------------------------------------

cmake_minimum_required(VERSION 3.23)

if(NOT APPLE)
    message(FATAL_ERROR "This version of the mlx support only MacOS")
endif()

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT HAS_PARENT)
    project(MinilibX)
endif()

enable_language(C)
enable_language(OBJC)

option(BUILD_SHARED_LIBS  "Build using shared libraries" OFF)
option(MLX_INCLUDE_EXEMPLES "Include exemple binaries in the project" OFF)

add_library(mlx
    "src/mlx_shaders.c" "src/mlx_new_window.m"
    "src/mlx_init_loop.m" "src/mlx_new_image.m"
    "src/mlx_xpm.c" "src/mlx_int_str_to_wordtab.c"
    "src/mlx_png.c" "src/mlx_mouse.m"
)

target_include_directories(mlx PRIVATE src PUBLIC include)

target_compile_options(mlx PRIVATE -w)

target_link_libraries(mlx INTERFACE "-framework AppKit" "-framework OpenGL")

if(MLX_INCLUDE_EXEMPLES)
    add_subdirectory(exemples)
endif()