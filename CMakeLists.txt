cmake_minimum_required(VERSION 3.16)
project(pds_lab5_reboot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(pds_lab5_reboot main.cpp ServerSocket.cpp ServerSocket.h Socket.h Socket.cpp Jobs.h Jobs.cpp Message.cpp Message.h)