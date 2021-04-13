cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME NES_EMULATOR)

project(${PROJECT_NAME})
set(_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
set(_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")
set(_THIRD_PARTY "${CMAKE_SOURCE_DIR}/ThirdParty")

set(CMAKE_CXX_STANDARD 14)

include_directories(${_INCLUDE_PATH})
include_directories(${_THIRD_PARTY})

# Projec source files
file(GLOB_RECURSE SRC_FILES
     "${_INCLUDE_PATH}/**.h"
     "${_SOURCE_PATH}/**.cpp"
)
file(GLOB_RECURSE THIRD_PARTY_FILES
     "${_INCLUDE_PATH}/**.h"
     "${_SOURCE_PATH}/**.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${THIRD_PARTY_FILES})

# Add SDL2 to project
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})



