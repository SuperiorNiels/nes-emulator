cmake_minimum_required(VERSION 3.16)

set(APP_NAME "6502_tester")

set(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE APP_SOURCES "${APP_PATH}/**.h" "${APP_PATH}/**.cpp")

# ---------------------------------- Build Targets ----------------------------------

add_executable(${APP_NAME} ${APP_SOURCES})
target_link_libraries(${APP_NAME} console_lib)
target_include_directories(${APP_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
