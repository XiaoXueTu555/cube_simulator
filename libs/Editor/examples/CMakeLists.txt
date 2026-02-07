# 测试目标 构建选项
option(APP_EXAMPLES_SINGLE_CUBE "build SingleCube example for test" ON)
option(APP_EXAMPLES_AXIS "build Axis example for test" OFF)


# 构建 GLFW 库
set(GLFW_DIRS "${LIBS}/third_party/glfw")

# 跳过构建 GLFW 中的多余目标，详情去看 glfw/CMakeLists.txt
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/glfw EXCLUDE_FROM_ALL)

# 确保安装了Vulkan SDK
find_package(Vulkan REQUIRED)

# 构建SingleCube测试目标
if (APP_EXAMPLES_SINGLE_CUBE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SingleCube)
endif ()

# 构建Axis测试目标
if (APP_EXAMPLES_AXIS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Axis)
endif ()


