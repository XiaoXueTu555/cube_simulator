cmake_minimum_required(VERSION 3.23)
project(Application)

set(CMAKE_CXX_STANDARD 20)

# libs 目录路径
set(LIBS "${CMAKE_CURRENT_SOURCE_DIR}/..")

# 构建 imgui
set(IMGUI_DIRS ${LIBS}/third_party/imgui)
set(IMGUI_BACKENDS_DIRS "${IMGUI_DIRS}/backends")
add_subdirectory(
        ${IMGUI_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}/ImGui
)

# 构建 Renderer库
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Renderer
        ${CMAKE_CURRENT_BINARY_DIR}/Renderer
)

# 构建Example库
add_library(Example)

target_sources(Example
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/examples/SingleCube.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/examples
            FILES   ${CMAKE_CURRENT_SOURCE_DIR}/examples/SingleCube.h
)
target_link_libraries(Example PUBLIC ImGui Renderer)
