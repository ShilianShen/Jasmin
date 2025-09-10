cmake_minimum_required(VERSION 3.30)
project(Jasmin C)
set(CMAKE_C_STANDARD 99)


find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/test/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES}
        src/outer/tempo/elem_type/text.c
        src/outer/tempo/elem_type/text.h
        src/outer/tempo/elem_type/slid.c
        src/outer/tempo/elem_type/slid.h
        src/outer/tempo/elem_type/bool.c
        src/outer/tempo/elem_type/bool.h
        src/outer/tempo/elem_type/file.c
        src/outer/tempo/elem_type/file.h)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)