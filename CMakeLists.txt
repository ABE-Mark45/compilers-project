cmake_minimum_required(VERSION 3.10)

project(Lexer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /usr/bin/g++-10)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(bin)