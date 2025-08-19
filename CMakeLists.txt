set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
cmake_minimum_required(VERSION 3.30)
project(jasmin C)


# 设置C标准
set(CMAKE_C_STANDARD 99)


# 添加可执行文件
file(GLOB SCR_FILES
        "${PROJECT_SOURCE_DIR}/tests/*.h"
        "${PROJECT_SOURCE_DIR}/tests/*.c"
)

# 查找库
find_package(Lua REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


add_executable(${CMAKE_PROJECT_NAME} ${SCR_FILES}
        src/outer/tempo/tempo.h
        src/inner/device/device.h
        src/inner/debug/debug.h
        src/inner/basic/setting/setting.h
        src/third_party/toml/toml.c
        src/outer/tempo/trig/trig.h
        src/outer/tempo/elem/elem.h
        src/outer/tempo/page/page.h
        src/inner/basic/basic.h
        src/inner/basic/setting/setting.c
        src/inner/debug/debug.c
        src/inner/device/device.c
        src/inner/basic/ease/ease.c
        src/inner/basic/ease/ease.h
        src/outer/tempo/elem/elem.c
        src/outer/tempo/interface.h
        src/outer/tempo/tempo.c
        src/outer/tempo/page/page.c
        src/outer/tempo/trig/trig.c
        src/inner/basic/interface.h
        src/outer/tempo/menu/menu.c
        src/outer/tempo/menu/menu.h
        src/outer/lopo/interface.h
        src/outer/lopo/lopo.c
        src/outer/lopo/lopo.h
        src/outer/lopo/shaders/debugShader.cpp
        src/outer/tempo/basic/basic.c
        src/outer/tempo/basic/basic.h
        include/jasmin/jasmin.h
        include/jasmin/tempo.h
        include/jasmin/lopo.h
        include/jasmin/basic.h
        src/inner/inner.h
        src/outer/interface.h
        src/third_party/third_party.h
        src/inner/interface.h
        src/outer/outer.h
        src/inner/basic/func/func.c
        src/inner/basic/func/func.h
        src/inner/basic/table/table.c
        src/inner/basic/table/table.h
        src/inner/basic/trigger/trigger.c
        src/inner/basic/trigger/trigger.h
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        ${LUA_INCLUDE_DIR}
)