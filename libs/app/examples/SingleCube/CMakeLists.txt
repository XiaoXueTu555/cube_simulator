# 构建Example库
add_executable(Example_SingleCube)

target_sources(Example_SingleCube
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/SingleCube.cpp
        PRIVATE
            ${IMGUI_BACKENDS_DIRS}/src/imgui_impl_glfw.cpp
            ${IMGUI_BACKENDS_DIRS}/src/imgui_impl_vulkan.cpp
            main.cpp
        PRIVATE
            FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
            FILES   ${CMAKE_CURRENT_SOURCE_DIR}/SingleCube.h
        PRIVATE
            FILE_SET    imgui_backends_header
            TYPE        HEADERS
            BASE_DIRS   ${IMGUI_BACKENDS_DIRS}/include
            FILES       ${IMGUI_BACKENDS_DIRS}/include/imgui_impl_glfw.h
                        ${IMGUI_BACKENDS_DIRS}/include/imgui_impl_vulkan.h
)
target_link_libraries(Example_SingleCube PRIVATE ImGui Renderer glfw Vulkan::Vulkan)
