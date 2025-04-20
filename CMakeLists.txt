cmake_minimum_required(VERSION 3.30)
project(Lo_Tri C)


# 设置C标准
set(CMAKE_C_STANDARD 11)


# 添加可执行文件
file(GLOB SCR_FILES
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable(${CMAKE_PROJECT_NAME} ${SCR_FILES})


# 查找库
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})