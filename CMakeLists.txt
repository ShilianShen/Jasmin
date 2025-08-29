cmake_minimum_required(VERSION 3.30)
project(Jasmin C)
set(CMAKE_C_STANDARD 99)


find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/tests/*.c"
        "${PROJECT_SOURCE_DIR}/src/inner/*.c"
        "${PROJECT_SOURCE_DIR}/src/outer/*.c"
        "${PROJECT_SOURCE_DIR}/src/third_party/cJSON-1.7.18/cJSON.c"
)


add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES}
        include/jasmin/inner.h
        include/jasmin/outer.h)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)