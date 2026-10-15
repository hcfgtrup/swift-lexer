cmake_minimum_required(VERSION 3.15)
project(SwiftLexer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lexer src/main.cpp)
target_compile_options(lexer PRIVATE -O2 -Wall -Wextra)
target_include_directories(lexer PRIVATE src)