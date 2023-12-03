cmake_minimum_required(VERSION 3.26)
project(cliente_servidor_threads C)

set(CMAKE_C_STANDARD 11)

link_libraries(ws2_32 wsock32)

add_executable(aula1_servidor src/aula1/servidor.c)
add_executable(aula1_cliente src/aula1/cliente.c)
add_executable(aula2_servidor src/aula2/servidor.c)
add_executable(aula2_cliente src/aula2/cliente.c)