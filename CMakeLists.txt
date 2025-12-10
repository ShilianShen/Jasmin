cmake_minimum_required(VERSION 3.30)
project(Jasmin C)
set(CMAKE_C_STANDARD 99)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")


add_subdirectory(src/third_party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(src/third_party/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(src/third_party/SDL_image EXCLUDE_FROM_ALL)


file(GLOB_RECURSE SRC_FILES
        "${PROJECT_SOURCE_DIR}/test/*.c"
        "${PROJECT_SOURCE_DIR}/src/basic/*.c"
        "${PROJECT_SOURCE_DIR}/src/debug/*.c"
        "${PROJECT_SOURCE_DIR}/src/perph/*.c"
        "${PROJECT_SOURCE_DIR}/src/intel/*.c"
        "${PROJECT_SOURCE_DIR}/src/tempo/*.c"
        "${PROJECT_SOURCE_DIR}/src/lotri/*.c"
        "${PROJECT_SOURCE_DIR}/src/third_party/cJSON-1.7.18/*.c"
        "${PROJECT_SOURCE_DIR}/src/third_party/fast_obj-1.3/*.c"
        "${PROJECT_SOURCE_DIR}/src/third_party/miniaudio-0.11.23/*.c"
)
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3_ttf::SDL3_ttf)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)