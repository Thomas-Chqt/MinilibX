# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 12:32:32
# ---------------------------------------------------

cmake_minimum_required(VERSION 3.23)

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT HAS_PARENT)
project(MinilibX-Multiplatform LANGUAGES CXX)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_library(mlx)

file(GLOB_RECURSE MLX_SOURCES "src/*.cpp" "src/*.tpp" "src/*.hpp" "include/*.tpp" "include/*.hpp" "include/*.h")

target_sources(mlx PRIVATE ${MLX_SOURCES})

target_include_directories(mlx PRIVATE src PUBLIC include)