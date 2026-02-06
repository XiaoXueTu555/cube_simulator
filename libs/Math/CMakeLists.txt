cmake_minimum_required(VERSION 3.23)
project(Math)

set(CMAKE_CXX_STANDARD 20)

option(TEST_MATH_DATA "Enable Test for Math" OFF)

# Vector
set(vector_header_files)
set(vector_source_files)

# Matrix
set(matrix_header_files)
set(matrix_source_files)

list(APPEND vector_header_files
        include/Math/vector3d.h
        include/Math/vector4d.h
)

list(APPEND matrix_header_files
        include/Math/matrix.h
        include/Math/scale_matrix.h
        include/Math/rotation_matrix.h
        include/Math/translation_matrix.h
        include/Math/view_matrix.h
        include/Math/perspective_projection_matrix.h
)

list(APPEND vector_source_files
        src/vector3d.cpp
        src/vector4d.cpp
)

list(APPEND matrix_source_files
        src/matrix.cpp
        src/scale_matrix.cpp
        src/rotation_matrix.cpp
        src/translation_matrix.cpp
        src/view_matrix.cpp
        src/perspective_projection_matrix.cpp
)


add_library(Math
        include/Math/rotator.h
        include/Math/quaternion.h
        src/quaternion.cpp
        src/rotator.cpp)

target_sources(Math
        PRIVATE
            ${vector_source_files}
            ${matrix_source_files}
        PUBLIC
            FILE_SET  math_header_files
            TYPE      HEADERS
            BASE_DIRS ./include/
            FILES     ${vector_header_files}
                      ${matrix_header_files}
)

if(${TEST_MATH_DATA})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Test)
endif ()
