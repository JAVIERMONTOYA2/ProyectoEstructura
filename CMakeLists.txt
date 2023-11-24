cmake_minimum_required(VERSION 3.21)
project(proyecto_estructura)

set(CMAKE_C_STANDARD 17)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/assets/SDL2-2.28.4/x86_64-w64-mingw32/lib/cmake/SDL2")
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/assets/SDL2-2.28.4/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)

add_executable(proyecto_estructura main.c
        list.c
        list.h
        Torreta.c
        Torreta.h
)

set(SDL_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/assets/SDL2_image-2.6.3/x86_64-w64-mingw32/include/SDL2")
set(SDL_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/assets/SDL2_image-2.6.3/x86_64-w64-mingw32/lib/libSDL2_image.a")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

include_directories(${SDL2TTF_INCLUDE_DIR})

include_directories(${SDL_IMAGE_INCLUDE_DIR})
target_link_libraries(proyecto_estructura PRIVATE ${SDL2_LIBRARIES} ${SDL_IMAGE_LIBRARY} -lRpcrt4)
target_link_libraries(proyecto_estructura PRIVATE SDL2::SDL2)



