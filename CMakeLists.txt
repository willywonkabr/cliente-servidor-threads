cmake_minimum_required(VERSION 3.26)
project(cliente_servidor_threads C)

set(CMAKE_C_STANDARD 11)

link_libraries(ws2_32 wsock32)

add_executable(cliente src/cliente.c)
add_executable(servidor src/servidor.c)