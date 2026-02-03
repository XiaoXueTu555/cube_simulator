cmake_minimum_required(VERSION 3.23)
project(Application)

set(CMAKE_CXX_STANDARD 20)

# 构建选项，构建例子
option(APP_ENABLE_EXAMPLES "Build Application examples" OFF)


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


# 构建 example
if (APP_ENABLE_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif ()
