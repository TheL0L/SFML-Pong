cmake_minimum_required(VERSION 3.10)

project(Pong)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Pong src/main.cpp)

target_link_libraries(Pong sfml-graphics sfml-window sfml-system)
