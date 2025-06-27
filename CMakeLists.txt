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
        src/jasmin/menu/struct/menu/menu.h
        src/jasmin/basic/device/device.h
        src/jasmin/basic/debug/debug.h
        src/jasmin/basic/setting/setting.h

        # toml/toml.h
        third_party/toml/toml.c
        src/jasmin/menu/struct/trig/trig.h
        src/jasmin/menu/struct/elem/elem.h
        src/jasmin/menu/struct/page/page.h
        src/jasmin/menu/menu.h
        src/jasmin/jasmin.h
        src/jasmin/basic/basic.h
        src/jasmin/basic/setting/setting.c
        src/jasmin/basic/debug/debug.c
        src/jasmin/basic/device/device.c
        src/jasmin/basic/ease/ease.c
        src/jasmin/basic/ease/ease.h
        src/jasmin/menu/func/draw.c
        src/jasmin/menu/struct/struct.h
        src/jasmin/menu/func/func.h
        src/jasmin/menu/struct/elem/elem.c
        src/jasmin/menu/menu_interface.h
        src/jasmin/menu/struct/interface.h
        src/jasmin/menu/func/interface.h
        src/jasmin/menu/struct/menu/menu.c
        src/jasmin/menu/struct/page/page.c
        src/jasmin/menu/struct/trig/trig.c
        src/jasmin/menu/func/init.c
        src/jasmin/menu/func/kill.c
        src/jasmin/menu/func/load.c
        src/jasmin/menu/func/renew.c
        src/jasmin/basic/interface.h
        src/jasmin/lo_tri/lo_tri.h
        src/jasmin/lo_tri/interface.h
        src/jasmin/lo_tri/test.c
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)