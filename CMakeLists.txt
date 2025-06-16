cmake_minimum_required(VERSION 3.30)
project(Lo_Tri C)


# 设置C标准
set(CMAKE_C_STANDARD 99)


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
        src/jasmin/menu/struct/menu.h
        src/jasmin/device.h
        src/jasmin/debug.h
        src/jasmin/basic.h

        # toml/toml.h
        third_party/toml/toml.c
        src/jasmin/menu/struct/trig.h
        src/jasmin/menu/struct/elem.h
        src/jasmin/menu/struct/page.h
        src/jasmin/menu/menu.h
        src/jasmin/menu/func/init.h
        src/jasmin/menu/func/load.h
        src/jasmin/menu/func/renew.h
        src/jasmin/menu/func/draw.h
        src/jasmin/menu/func/kill.h
        src/jasmin/menu/func/test.h
        src/jasmin/menu/func/trig.h
        src/jasmin/jasmin.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)