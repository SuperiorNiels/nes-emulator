cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME NES_EMULATOR)

project(${PROJECT_NAME})
set(_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
set(_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 14)

include_directories(${_INCLUDE_PATH})

file(GLOB_RECURSE SRC_FILES
     "${_INCLUDE_PATH}/**.h"
     "${_SOURCE_PATH}/**.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})



