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
        modules/tempo/tempo.h
        modules/basic/device/device.h
        modules/basic/debug/debug.h
        modules/basic/setting/setting.h

        # toml/toml.h
        third_party/toml/toml.c
        modules/tempo/trig/trig.h
        modules/tempo/elem/elem.h
        modules/tempo/page/page.h
        jasmin.h
        modules/basic/basic.h
        modules/basic/setting/setting.c
        modules/basic/debug/debug.c
        modules/basic/device/device.c
        modules/basic/ease/ease.c
        modules/basic/ease/ease.h
        modules/tempo/elem/elem.c
        modules/tempo/interface.h
        modules/tempo/tempo.c
        modules/tempo/page/page.c
        modules/tempo/trig/trig.c
        modules/basic/interface.h
        modules/lo_tri/lo_tri.h
        modules/lo_tri/interface.h
        modules/lo_tri/model/model.c
        modules/lo_tri/model/model.h
        modules/lo_tri/camera/camera.c
        modules/lo_tri/camera/camera.h
        modules/lo_tri/pipline/pipline.c
        modules/lo_tri/pipline/pipline.h
        modules/lo_tri/model/bone/bone.c
        modules/lo_tri/model/bone/bone.h
        modules/lo_tri/model/mesh/mesh.c
        modules/lo_tri/model/interface.h
        modules/lo_tri/model/mesh/mesh.h
        modules/lo_tri/math/math.c
        modules/lo_tri/math/math.h
        modules/lo_tri/math/interface.h
        modules/lo_tri/math/mat/mat.c
        modules/lo_tri/math/mat/mat.h
        modules/lo_tri/math/vec/vec.c
        modules/lo_tri/math/vec/vec.h
        modules/tempo/menu/menu.c
        modules/tempo/menu/menu.h
        modules/lo_tri/zbuffer/zbuffer.c
        modules/lo_tri/zbuffer/zbuffer.h
        modules/lo_tri/lopo.c
        modules/lopo/interface.h
        modules/lopo/lopo.c
        modules/lopo/lopo.h
        modules/lopo/shaders/debugShader.cpp
        tests/maze_inner/maze_inner.c
        tests/maze_inner/maze_inner.h
        tests/maze_inner/interface.h
        tests/maze_inner/monster/monster.c
        tests/maze_inner/monster/monster.h
        tests/maze_inner/gene/gene.c
        tests/maze_inner/gene/gene.h
        tests/maze_inner/room/room.c
        tests/maze_inner/room/room.h
        tests/maze_inner/basic/basic.c
        tests/maze_inner/basic/basic.h
        modules/tempo/basic/basic.c
        modules/tempo/basic/basic.h
        tests/maze_outer/interface.h
        tests/maze_outer/maze_outer.c
        tests/maze_outer/maze_outer.h
        tests/maze_outer/zero/zero.c
        tests/maze_outer/zero/zero.h
        tests/maze_inner/action/action.c
        tests/maze_inner/action/action.h
        interface.h
        tests/maze_outer/zero/RDI/RDI.c
        tests/maze_outer/zero/RDI/RDI.h
        tests/maze_outer/zero/interface.h
        tests/maze_outer/zero/room/room.c
        tests/maze_outer/zero/room/room.h
        tests/maze_inner/item/item.c
        tests/maze_inner/item/item.h
        tests/maze_inner/coord/coord.c
        tests/maze_inner/coord/coord.h
        tests/maze_outer/zero/you/you.c
        tests/maze_outer/zero/you/you.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)