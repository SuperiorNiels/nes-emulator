cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME nes_emulator)

project(${PROJECT_NAME})
set(_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
set(_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")
set(_IMGUI_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")

set(CMAKE_CXX_STANDARD 14)
set(GCC_COMPILE_FLAGS "")
SET(GCC_LINK_FLAGS "-limm32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")
add_link_options("-limm32")

include_directories(${_INCLUDE_PATH})
include_directories(${_IMGUI_PATH})

# Projec source files
file(GLOB_RECURSE SRC_FILES
     "${_INCLUDE_PATH}/**.h"
     "${_SOURCE_PATH}/**.cpp"
)
file(GLOB_RECURSE IMGUI_FILES
     "${_IMGUI_PATH}/**.h"
     "${_IMGUI_PATH}/**.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
pkg_search_module(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})


