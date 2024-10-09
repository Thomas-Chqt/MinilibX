# ---------------------------------------------------
# CMakeLists.txt
#
# Author: Thomas Choquet <thomas.publique@icloud.com>
# Date: 2024/05/25 12:32:32
# ---------------------------------------------------

cmake_minimum_required(VERSION 3.16)

option(BUILD_SHARED_LIBS  "Build using shared libraries"       OFF)
option(MLX_BUILD_EXAMPLES "Build MinilibX example executables" OFF)

if (APPLE)
    option(MLX_FORCE_OPENGL "Use opengl" OFF)
endif()

if(NOT APPLE OR MLX_FORCE_OPENGL)
    set(MLX_USING_OPENGL ON)
else()
    set(MLX_USING_METAL ON)
endif()



project(MinilibX)

enable_language(CXX)


add_library(mlx ${MLX_LIBRARY_TYPE})

set_target_properties(mlx PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE MLX_SOURCES "src/*.cpp" "src/*.tpp" "src/*.hpp" "include/*.tpp" "include/*.hpp" "include/*.h")
target_sources(mlx PRIVATE ${MLX_SOURCES})


target_include_directories(mlx PRIVATE "src" PUBLIC "include")


if(MLX_USING_METAL)
    set(MTL_SHADER_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Shaders")
    set(MTL_SHADER_BUILD_DIR  "${CMAKE_CURRENT_BINARY_DIR}")
    get_target_property(MLX_INCLUDE_DIRS mlx INCLUDE_DIRECTORIES) 
    foreach(dir IN LISTS MLX_INCLUDE_DIRS)
        list(APPEND MTL_SHADER_BUILD_FLAGS "-I${dir}")
    endforeach()
    set(MTL_SHADER_LIB "${MTL_SHADER_BUILD_DIR}/MetalShaderLibrary.metallib")

    target_compile_definitions(mlx PRIVATE "MTL_SHADER_LIB=\"${MTL_SHADER_LIB}\"")
    target_compile_definitions(mlx PRIVATE "MLX_USING_METAL")

    add_custom_target(MLX_MetalShaderLibrary DEPENDS ${MTL_SHADER_LIB} SOURCES ${MTL_SHADER_LIB_SRCS})
    add_dependencies(mlx MLX_MetalShaderLibrary)

    find_program(XCRUN_PATH xcrun REQUIRED)
    file(GLOB_RECURSE MTL_SHADER_LIB_SRCS "${MTL_SHADER_SOURCE_DIR}/*.metal")    
    foreach(src IN LISTS MTL_SHADER_LIB_SRCS)
        string(REPLACE ".metal" ".air" air "${src}")
        string(REPLACE ${MTL_SHADER_SOURCE_DIR} ${MTL_SHADER_BUILD_DIR} air "${air}")
        add_custom_command(OUTPUT "${air}" COMMAND ${XCRUN_PATH} -sdk macosx metal ${MTL_SHADER_BUILD_FLAGS} -o "${air}" -c "${src}" DEPENDS "${src}")
        list(APPEND MTL_SHADER_LIB_AIRS "${air}")
    endforeach()
    add_custom_command(OUTPUT ${MTL_SHADER_LIB} COMMAND ${XCRUN_PATH} -sdk macosx metallib -o ${MTL_SHADER_LIB} ${MTL_SHADER_LIB_AIRS} DEPENDS ${MTL_SHADER_LIB_AIRS})
elseif(MLX_USING_OPENGL)
    target_compile_definitions(mlx PRIVATE "OPENGL_SHADER_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/Shaders\"")
    target_compile_definitions(mlx PRIVATE "MLX_USING_OPENGL")
endif()

add_subdirectory("dependencies")

if(MLX_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()