cmake_minimum_required(VERSION 3.23)
project(Renderer)

set(CMAKE_CXX_STANDARD 20)

# 构建测试目标
option(TEST_RENDERER "Build Test target for Renderer" OFF)

# 构建 SceneData库
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../SceneData
        ${CMAKE_CURRENT_BINARY_DIR}/SceneData
)

# 构建 Renderer 库
add_library(Renderer
        src/view_port.cpp)

target_sources(Renderer
        PRIVATE
            src/scene_renderer.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
            FILES
                include/Renderer/scene_renderer.h
)

target_link_libraries(Renderer PUBLIC SceneData)


# 构建测试目标
if (${TEST_RENDERER})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
endif ()
