cmake_minimum_required(VERSION 3.16)

# Config
set(PROJECT_NAME nes_emulator)
project(${PROJECT_NAME})
set(SRC_PATH "${CMAKE_SOURCE_DIR}/src")
set(MAIN_SRC "${CMAKE_SOURCE_DIR}/main.cpp")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/tests")
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/lib/imgui")
set(IMGUI_CLUB_PATH "${CMAKE_SOURCE_DIR}/lib/imgui_club")
set(IMGUI_FILE_DIALOG_PATH "${CMAKE_SOURCE_DIR}/lib/imgui_filedialog")
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
enable_testing()
include(GoogleTest)
include(FindPkgConfig)

# ---------------------------------- Sources and Libraries ----------------------------------

# Project source files
file(GLOB_RECURSE SRC_FILES
     "${SRC_PATH}/**.h"
     "${SRC_PATH}/**.cpp"
)
include_directories("${SRC_PATH}/include")

# Testing source files
file(GLOB_RECURSE TEST_FILES
     "${TEST_PATH}/**.cpp"
)

# SDL2
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

# opengl
find_package(OpenGL REQUIRED)
pkg_search_module(GLEW REQUIRED glew)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

# imgui
set(IMGUI_BACKEND_FILES 
     "${IMGUI_PATH}/backends/imgui_impl_sdl.h"
     "${IMGUI_PATH}/backends/imgui_impl_sdl.cpp"
     "${IMGUI_PATH}/backends/imgui_impl_opengl3.h"
     "${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
)
file(GLOB IMGUI_FILES "${IMGUI_PATH}/*.cpp" "${IMGUI_PATH}/*.h")
file(GLOB_RECURSE IMGUI_CLUB_FILES "${IMGUI_CLUB_PATH}/*.cpp" "${IMGUI_CLUB_PATH}/*.h")
file(GLOB_RECURSE IMGUI_FILE_DIALOG_FILES "${IMGUI_FILE_DIALOG_PATH}/*.cpp" "${IMGUI_FILE_DIALOG_PATH}/*.h")
include_directories(${IMGUI_PATH})
include_directories(${IMGUI_CLUB_PATH})
include_directories(${IMGUI_FILE_DIALOG_PATH})
include_directories("${IMGUI_PATH}/backends")

# ---------------------------------- Build Targets ----------------------------------

# Console library
add_library("${PROJECT_NAME}_lib" ${SRC_FILES})
target_link_libraries("${PROJECT_NAME}_lib" ${SDL2_LIBRARIES})

# GTest EXE
add_executable("${PROJECT_NAME}_test" ${TEST_FILES})
target_link_libraries("${PROJECT_NAME}_test" "${PROJECT_NAME}_lib" gtest_main)
gtest_discover_tests("${PROJECT_NAME}_test")

# imgui lib
add_library("${PROJECT_NAME}_imgui_lib" ${IMGUI_FILES} ${IMGUI_BACKEND_FILES} ${IMGUI_CLUB_FILES} ${IMGUI_FILE_DIALOG_FILES})
target_link_libraries("${PROJECT_NAME}_imgui_lib" ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
# target_link_options("${PROJECT_NAME}_imgui_lib" PUBLIC "-limm32")
# Workaround, mingw cant seem to find and link to imm32 library 
target_compile_definitions("${PROJECT_NAME}_imgui_lib" PUBLIC IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS)

# imgui example
add_executable("${PROJECT_NAME}_imgui_example" "./lib/imgui/examples/example_sdl_opengl3/main.cpp")
target_link_libraries("${PROJECT_NAME}_imgui_example" "${PROJECT_NAME}_imgui_lib")

# Emulator EXE
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} "${PROJECT_NAME}_lib" "${PROJECT_NAME}_imgui_lib")