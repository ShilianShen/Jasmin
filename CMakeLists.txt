cmake_minimum_required(VERSION 3.30)
project(Jasmin C)
set(CMAKE_C_STANDARD 99)


find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(OpenGL REQUIRED)


file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/test/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        OpenGL::GL
)