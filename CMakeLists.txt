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
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)


add_executable(${CMAKE_PROJECT_NAME} ${SCR_FILES}
        src/tempo/tempo.h
        src/basic/device/device.h
        src/basic/debug/debug.h
        src/basic/setting/setting.h

        # toml/toml.h
        third_party/toml/toml.c
        src/tempo/trig/trig.h
        src/tempo/elem/elem.h
        src/tempo/page/page.h
        src/basic/basic.h
        src/basic/setting/setting.c
        src/basic/debug/debug.c
        src/basic/device/device.c
        src/basic/ease/ease.c
        src/basic/ease/ease.h
        src/tempo/elem/elem.c
        src/tempo/interface.h
        src/tempo/tempo.c
        src/tempo/page/page.c
        src/tempo/trig/trig.c
        src/basic/interface.h
        src/lo_tri/lo_tri.h
        src/lo_tri/interface.h
        src/lo_tri/model/model.c
        src/lo_tri/model/model.h
        src/lo_tri/camera/camera.c
        src/lo_tri/camera/camera.h
        src/lo_tri/pipline/pipline.c
        src/lo_tri/pipline/pipline.h
        src/lo_tri/model/bone/bone.c
        src/lo_tri/model/bone/bone.h
        src/lo_tri/model/mesh/mesh.c
        src/lo_tri/model/interface.h
        src/lo_tri/model/mesh/mesh.h
        src/lo_tri/math/math.c
        src/lo_tri/math/math.h
        src/lo_tri/math/interface.h
        src/lo_tri/math/mat/mat.c
        src/lo_tri/math/mat/mat.h
        src/lo_tri/math/vec/vec.c
        src/lo_tri/math/vec/vec.h
        src/tempo/menu/menu.c
        src/tempo/menu/menu.h
        src/lo_tri/zbuffer/zbuffer.c
        src/lo_tri/zbuffer/zbuffer.h
        src/lo_tri/lopo.c
        src/lopo/interface.h
        src/lopo/lopo.c
        src/lopo/lopo.h
        src/lopo/shaders/debugShader.cpp

        src/tempo/basic/basic.c
        src/tempo/basic/basic.h

        include/jasmin/jasmin.h
        include/jasmin/tempo.h
        include/jasmin/lopo.h
        include/jasmin/basic.h
        include/jasmin/lotri.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)