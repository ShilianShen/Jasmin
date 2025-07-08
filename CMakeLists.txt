set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk")
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
        src/jasmin/menu/menu.h
        src/jasmin/basic/device/device.h
        src/jasmin/basic/debug/debug.h
        src/jasmin/basic/setting/setting.h

        # toml/toml.h
        third_party/toml/toml.c
        src/jasmin/menu/trig/trig.h
        src/jasmin/menu/elem/elem.h
        src/jasmin/menu/page/page.h
        src/jasmin/jasmin.h
        src/jasmin/basic/basic.h
        src/jasmin/basic/setting/setting.c
        src/jasmin/basic/debug/debug.c
        src/jasmin/basic/device/device.c
        src/jasmin/basic/ease/ease.c
        src/jasmin/basic/ease/ease.h
        src/jasmin/menu/elem/elem.c
        src/jasmin/menu/interface.h
        src/jasmin/menu/menu.c
        src/jasmin/menu/page/page.c
        src/jasmin/menu/trig/trig.c
        src/jasmin/basic/interface.h
        src/jasmin/lo_tri/lo_tri.h
        src/jasmin/lo_tri/interface.h
        src/jasmin/lo_tri/test.c
        src/jasmin/lo_tri/model/model.c
        src/jasmin/lo_tri/model/model.h
        src/jasmin/lo_tri/camera/camera.c
        src/jasmin/lo_tri/camera/camera.h
        src/jasmin/lo_tri/pipline/pipline.c
        src/jasmin/lo_tri/pipline/pipline.h
        src/jasmin/lo_tri/draw/draw.c
        src/jasmin/lo_tri/draw/draw.h
        src/jasmin/lo_tri/model/bone/bone.c
        src/jasmin/lo_tri/model/bone/bone.h
        src/jasmin/lo_tri/model/mesh/mesh.c
        src/jasmin/lo_tri/model/interface.h
        src/jasmin/lo_tri/model/mesh/mesh.h
        src/jasmin/lo_tri/math/math.c
        src/jasmin/lo_tri/math/math.h
        src/jasmin/lo_tri/math/interface.h
        src/jasmin/lo_tri/math/mat/mat.c
        src/jasmin/lo_tri/math/mat/mat.h
        src/jasmin/lo_tri/math/vec/vec.c
        src/jasmin/lo_tri/math/vec/vec.h
        src/jasmin/menu/menu/menu.c
        src/jasmin/menu/menu/menu.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)