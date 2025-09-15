cmake_minimum_required(VERSION 3.22)
project(game C)

set(CMAKE_C_STANDARD 11)


add_executable(server server/server.c server/server.h
        common/common.h
        common/common.cpp)

add_executable(client client/client.c client/client.h
        common/common.h
        common/common.cpp)