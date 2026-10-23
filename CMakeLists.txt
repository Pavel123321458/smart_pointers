cmake_minimum_required(VERSION 3.10)
project(smart_pointers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE -fsanitize=address,undefined -g)
target_link_options(main PRIVATE -fsanitize=address,undefined)