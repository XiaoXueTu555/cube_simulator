cmake_minimum_required(VERSION 3.23)
project(SceneData)

set(CMAKE_CXX_STANDARD 20)

option(TEST_SCENE_DATA "Enable Test for SceneData" OFF)

# 构建 Math 库
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Math
        ${CMAKE_CURRENT_BINARY_DIR}/Math
)

# 定义 SceneData 库
add_library(SceneData
        src/camera.cpp
        src/mesh.cpp
        src/material_manager.cpp)

target_sources(SceneData
        PRIVATE
            src/transform.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS ./include
            FILES include/SceneData/transform.h
                  include/SceneData/camera.h
                  include/SceneData/mesh.h
                  include/SceneData/material_manager.h
                  include/SceneData/model.h
)

target_link_libraries(SceneData PUBLIC Math)

# 构建 Test executable 为了 SceneData

if (${TEST_SCENE_DATA})
    add_executable(TestSceneData
            # Test/MeshTest.cpp
            Test/material_manager.cpp
    )

    target_link_libraries(TestSceneData SceneData)
endif ()
