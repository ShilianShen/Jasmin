cmake_minimum_required(VERSION 3.12)

project(Jasmin)

set(CMAKE_C_STANDARD 99)

set(OUTPUT_ROOT_DIR ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_ROOT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_ROOT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_ROOT_DIR})

file(GLOB_RECURSE SRC
    ${CMAKE_SOURCE_DIR}/src/*.c
#     ${CMAKE_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE TEST
   ${CMAKE_SOURCE_DIR}/test/*.c
#    ${CMAKE_SOURCE_DIR}/test/*.h
)

add_executable(${PROJECT_NAME} ${TEST} ${SRC})

file(GLOB_RECURSE DLL_DIR 
    ${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.24/x86_64-w64-mingw32/bin/*.dll
    ${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin/*.dll
    ${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/bin/*.dll)
    
file(COPY ${DLL_DIR}
    DESTINATION ${OUTPUT_ROOT_DIR}
)

# target_link_libraries(${PROJECT_NAME} PUBLIC opengl32.lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.24/x86_64-w64-mingw32/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.24/x86_64-w64-mingw32/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC libSDL3.dll.a)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC libSDL3_ttf.dll.a)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC libSDL3_image.dll.a)


