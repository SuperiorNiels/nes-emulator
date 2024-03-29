cmake_minimum_required(VERSION 3.16)

# Config
set(PROJECT_NAME nes_emulator)
project(${PROJECT_NAME})
set(SRC_PATH "${CMAKE_SOURCE_DIR}")
set(APPS_PATH "${CMAKE_SOURCE_DIR}/apps")
set(TEST_PATH "${CMAKE_SOURCE_DIR}/tests")
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/lib/imgui")
set(IMGUI_CLUB_PATH "${CMAKE_SOURCE_DIR}/lib/imgui_club")
set(IMGUI_FILE_DIALOG_PATH "${CMAKE_SOURCE_DIR}/lib/imgui_filedialog")
set(CMAKE_CXX_STANDARD 14)

if(WIN32) 
     add_compile_options("-mwindows")
endif()

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
     "${SRC_PATH}/include/*.h"
     "${SRC_PATH}/src/*.cpp"
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
add_library(console_lib ${SRC_FILES})
target_link_libraries(console_lib ${SDL2_LIBRARIES})

# GTest EXE
add_executable(console_lib_test ${TEST_FILES})
target_link_libraries(console_lib_test console_lib gtest_main)
gtest_discover_tests(console_lib_test)

# imgui lib
add_library(imgui_lib ${IMGUI_FILES} ${IMGUI_BACKEND_FILES} ${IMGUI_CLUB_FILES} ${IMGUI_FILE_DIALOG_FILES})
target_link_libraries(imgui_lib ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
# target_link_options("${PROJECT_NAME}_imgui_lib" PUBLIC "-limm32")
# Workaround, mingw cant seem to find and link to imm32 library 
target_compile_definitions(imgui_lib PUBLIC IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS)

# imgui example
add_executable(imgui_example "./lib/imgui/examples/example_sdl_opengl3/main.cpp")
target_link_libraries(imgui_example imgui_lib)

# add app subdirs
FILE(GLOB APPS "${APPS_PATH}/**")
foreach(APP ${APPS})
     add_subdirectory(${APP})
     message("App added: ${APP}")
endforeach()

