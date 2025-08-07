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
        src/modules/tempo/tempo.h
        src/modules/basic/device/device.h
        src/modules/basic/debug/debug.h
        src/modules/basic/setting/setting.h

        # toml/toml.h
        third_party/toml/toml.c
        src/modules/tempo/trig/trig.h
        src/modules/tempo/elem/elem.h
        src/modules/tempo/page/page.h
        src/jasmin.h
        src/modules/basic/basic.h
        src/modules/basic/setting/setting.c
        src/modules/basic/debug/debug.c
        src/modules/basic/device/device.c
        src/modules/basic/ease/ease.c
        src/modules/basic/ease/ease.h
        src/modules/tempo/elem/elem.c
        src/modules/tempo/interface.h
        src/modules/tempo/tempo.c
        src/modules/tempo/page/page.c
        src/modules/tempo/trig/trig.c
        src/modules/basic/interface.h
        src/modules/lo_tri/lo_tri.h
        src/modules/lo_tri/interface.h
        src/modules/lo_tri/model/model.c
        src/modules/lo_tri/model/model.h
        src/modules/lo_tri/camera/camera.c
        src/modules/lo_tri/camera/camera.h
        src/modules/lo_tri/pipline/pipline.c
        src/modules/lo_tri/pipline/pipline.h
        src/modules/lo_tri/model/bone/bone.c
        src/modules/lo_tri/model/bone/bone.h
        src/modules/lo_tri/model/mesh/mesh.c
        src/modules/lo_tri/model/interface.h
        src/modules/lo_tri/model/mesh/mesh.h
        src/modules/lo_tri/math/math.c
        src/modules/lo_tri/math/math.h
        src/modules/lo_tri/math/interface.h
        src/modules/lo_tri/math/mat/mat.c
        src/modules/lo_tri/math/mat/mat.h
        src/modules/lo_tri/math/vec/vec.c
        src/modules/lo_tri/math/vec/vec.h
        src/modules/tempo/menu/menu.c
        src/modules/tempo/menu/menu.h
        src/modules/lo_tri/zbuffer/zbuffer.c
        src/modules/lo_tri/zbuffer/zbuffer.h
        src/modules/lo_tri/lopo.c
        src/modules/lopo/interface.h
        src/modules/lopo/lopo.c
        src/modules/lopo/lopo.h
        src/modules/lopo/shaders/debugShader.cpp
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
        src/modules/tempo/basic/basic.c
        src/modules/tempo/basic/basic.h
        tests/maze_outer/interface.h
        tests/maze_outer/maze_outer.c
        tests/maze_outer/maze_outer.h
        tests/maze_outer/zero/zero.c
        tests/maze_outer/zero/zero.h
        tests/maze_inner/action/action.c
        tests/maze_inner/action/action.h
        src/interface.h
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
        include/jasmin/jasmin.h
        include/jasmin/tempo.h
        include/jasmin/lopo.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
)