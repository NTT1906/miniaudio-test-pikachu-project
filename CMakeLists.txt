cmake_minimum_required(VERSION 3.27)
project(Pokachu_MUSIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Pokachu_MUSIC main.cpp
        miniaudio.h)
