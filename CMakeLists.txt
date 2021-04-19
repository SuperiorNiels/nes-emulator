cmake_minimum_required(VERSION 3.20)

# Config
set(PROJECT_NAME nes_emulator)
project(${PROJECT_NAME})
set(SRC_PATH "${CMAKE_SOURCE_DIR}/src")
set(MAIN_SRC "${CMAKE_SOURCE_DIR}/main.cpp")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/tests")
set(CMAKE_CXX_STANDARD 14)

# Google GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---------------------------------- Sources and Libraries ----------------------------------
# Projec source files
include_directories("${SRC_PATH}/include")
file(GLOB_RECURSE SRC_FILES
     "${SRC_PATH}/**.h"
     "${SRC_PATH}/**.cpp"
)

# Testing source files
enable_testing()
include(GoogleTest)
# Projec source files
file(GLOB_RECURSE TEST_FILES
     "${TEST_PATH}/**.cpp"
)

# Add SDL2
include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# ---------------------------------- Build Targets ----------------------------------

# Console library
add_library("${PROJECT_NAME}_lib" ${SRC_FILES})
target_link_libraries("${PROJECT_NAME}_lib" ${SDL2_LIBRARIES})

# Emulator EXE
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} "${PROJECT_NAME}_lib")

# GTest EXE
add_executable("${PROJECT_NAME}_test" ${TEST_FILES})
target_link_libraries("${PROJECT_NAME}_test" "${PROJECT_NAME}_lib" gtest_main)
gtest_discover_tests("${PROJECT_NAME}_test")
