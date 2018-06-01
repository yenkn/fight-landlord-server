cmake_minimum_required(VERSION 3.9)
project(fight_landlord_server)

set(CMAKE_CXX_STANDARD 11)

add_executable(fight_landlord_server main.cpp TcpServer.cpp TcpServer.h TcpClient.cpp TcpClient.h MessageClient.cpp MessageClient.h)
target_link_libraries(fight_landlord_server libev.a)