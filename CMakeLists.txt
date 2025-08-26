set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
cmake_minimum_required(VERSION 3.30)
project(jasmin C)

# 设置C标准
set(CMAKE_C_STANDARD 99)


# 强制设置编译标志，覆盖任何外部设置
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")



# 添加可执行文件
file(GLOB SCR_FILES
        "${PROJECT_SOURCE_DIR}/tests/*.h"
        "${PROJECT_SOURCE_DIR}/tests/*.c"
)

# 查找库
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


add_executable(${CMAKE_PROJECT_NAME} ${SCR_FILES}
        src/outer/tempo/tempo.h
        src/inner/device/device.h
        src/inner/debug/debug.h
        src/inner/basic/setting.h
        src/third_party/cJSON-1.7.18/cJSON.c
        src/third_party/cJSON-1.7.18/cJSON.h
        src/outer/tempo/elem.h
        src/outer/tempo/page.h
        src/inner/basic/basic.h
        src/inner/basic/setting.c
        src/inner/debug/debug.c
        src/inner/device/device.c
        src/inner/basic/ease.c
        src/inner/basic/ease.h
        src/outer/tempo/elem.c
        src/outer/tempo/interface.h
        src/outer/tempo/tempo.c
        src/outer/tempo/page.c
        src/inner/basic/interface.h
        src/outer/tempo/menu.c
        src/outer/tempo/menu.h
        src/outer/tempo/theme.c
        src/outer/tempo/theme.h
        include/jasmin/jasmin.h
        include/jasmin/tempo.h
        include/jasmin/basic.h
        src/inner/inner.h
        src/outer/interface.h
        src/third_party/third_party.h
        src/inner/interface.h
        src/outer/outer.h
        src/inner/basic/func.c
        src/inner/basic/func.h
        src/inner/basic/table.c
        src/inner/basic/table.h
        src/inner/basic/trigger.c
        src/inner/basic/trigger.h
        tests/tempo_interface.c
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)