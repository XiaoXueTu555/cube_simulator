cmake_minimum_required(VERSION 3.23)
project(cube_simulator)

set(CMAKE_CXX_STANDARD 20)

# 构建 GLFW 库
set(GLFW_DIRS "./libs/third_party/glfw")

# 跳过构建 GLFW 中的多余目标，详情去看 glfw/CMakeLists.txt
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/glfw EXCLUDE_FROM_ALL)

# 确保安装了Vulkan SDK
find_package(Vulkan REQUIRED)

# 构建Application库
add_subdirectory(libs/app)

# 总结所有的库
set(LIBRARYS "glfw;Vulkan::Vulkan;ImGui")

add_executable(cube_simulator)

# 指定ImGui的目录
set(IMGUI_DIRS "./libs/third_party/imgui")
set(IMGUI_BACKENDS_DIRS "${IMGUI_DIRS}/backends")

target_sources(cube_simulator
    PRIVATE
        ${IMGUI_BACKENDS_DIRS}/src/imgui_impl_glfw.cpp
        ${IMGUI_BACKENDS_DIRS}/src/imgui_impl_vulkan.cpp
        main.cpp
    PRIVATE
        FILE_SET    imgui_backends_header
        TYPE        HEADERS
        BASE_DIRS   ${IMGUI_BACKENDS_DIRS}/include
        FILES       ${IMGUI_BACKENDS_DIRS}/include/imgui_impl_glfw.h
                    ${IMGUI_BACKENDS_DIRS}/include/imgui_impl_vulkan.h
)

target_link_libraries(cube_simulator ${LIBRARYS})
