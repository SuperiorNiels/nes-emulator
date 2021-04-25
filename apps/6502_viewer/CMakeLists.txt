cmake_minimum_required(VERSION 3.16)

set(APP_NAME "6502_viewer")

set(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB ICON_PATH "${APP_PATH}/icon/*.o")
file(GLOB_RECURSE APP_SOURCES "${APP_PATH}/**.h" "${APP_PATH}/**.cpp")

# ---------------------------------- Build Targets ----------------------------------

# 6502 viewer EXE
add_executable(${APP_NAME} ${APP_SOURCES} ${ICON_PATH})
target_link_libraries(${APP_NAME} console_lib imgui_lib)