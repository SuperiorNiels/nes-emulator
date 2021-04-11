cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME NES_EMULATOR)

project(${PROJECT_NAME})
set(_SOURCE_PATH "${CMAKE_SOURCE_DIR}/src")
set(_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

file(GLOB SRC_FILES
     "${_INCLUDE_PATH}/**.h"
     "${_SOURCE_PATH}/**.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES})
include_directories(${_INCLUDE_PATH})

find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

