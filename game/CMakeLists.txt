cmake_minimum_required(VERSION 3.22)
project(game C)

set(CMAKE_C_STANDARD 11)

#set(DATA_FILE "data/item.dat")

file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data)

add_executable(server server/server.c server/server.h
        common/common.h
        common/common.c)

add_executable(client client/client.c client/client.h
        common/common.h
        common/common.c)