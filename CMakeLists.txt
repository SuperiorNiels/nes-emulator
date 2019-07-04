cmake_minimum_required(VERSION 3.12)
project(nes)

set(CMAKE_CXX_STANDARD 14)

add_executable(nes main.cpp CPU.cpp CPU.h Memory.cpp Memory.h Console.cpp Console.h)