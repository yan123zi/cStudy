cmake_minimum_required(VERSION 3.28)
project(game C)

set(CMAKE_C_STANDARD 11)


add_executable(server server/server.c server/server.h)

add_executable(client client/client.c client/client.h)