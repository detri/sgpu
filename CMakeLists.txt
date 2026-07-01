cmake_minimum_required(VERSION 3.23)
project(sgpu VERSION 0.1.0 LANGUAGES CXX)

set(DETRI_SDL_VENDORED OFF CACHE BOOL "Whether to vendor SDL")

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 CONFIG QUIET)
endif()

if(NOT TARGET SDL3::SDL3 AND (PROJECT_IS_TOP_LEVEL OR DETRI_SDL_VENDORED))
    include(FetchContent)
    FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-3.4.10
    )
    FetchContent_MakeAvailable(SDL)
elseif(NOT TARGET SDL3::SDL3)
    message(FATAL_ERROR "SDL3::SDL3 must already exist when sgpu is used as a subproject without vendoring")
endif()

add_library(sgpu STATIC)
add_library(sgpu::sgpu ALIAS sgpu)

target_compile_features(sgpu PUBLIC cxx_std_23)

set_target_properties(sgpu PROPERTIES
    CXX_EXTENSIONS OFF
)

target_sources(sgpu PRIVATE
    src/buffer.cpp
    src/command_buffer.cpp
    src/compute_pass.cpp
    src/compute_pipeline.cpp
    src/copy_pass.cpp
    src/device.cpp
    src/graphics_pipeline.cpp
    src/render_pass.cpp
    src/sampler.cpp
    src/shader.cpp
    src/texture.cpp
    src/transfer_buffer.cpp
)

target_sources(sgpu PUBLIC FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/sgpu/buffer.hpp
        include/sgpu/command_buffer.hpp
        include/sgpu/compute_pass.hpp
        include/sgpu/compute_pipeline.hpp
        include/sgpu/copy_pass.hpp
        include/sgpu/device.hpp
        include/sgpu/graphics_pipeline.hpp
        include/sgpu/handle.hpp
        include/sgpu/pass.hpp
        include/sgpu/render_pass.hpp
        include/sgpu/resource.hpp
        include/sgpu/sampler.hpp
        include/sgpu/sgpu.hpp
        include/sgpu/shader.hpp
        include/sgpu/texture.hpp
        include/sgpu/transfer_buffer.hpp
)

# sgpu does not hide SDL3 because it only wraps part of it
target_link_libraries(sgpu PUBLIC SDL3::SDL3)
