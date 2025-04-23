cmake_minimum_required(VERSION 3.30)
project(Lo_Tri C)


# 设置C标准
set(CMAKE_C_STANDARD 90)


# 添加可执行文件
file(GLOB SCR_FILES
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
)

# 查找库
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${SCR_FILES}
        src/menu.h
        src/device.h
        src/debug.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)