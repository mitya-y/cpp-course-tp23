cmake_minimum_required(VERSION 3.16)
project(cpp_lambdas)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lambdas lambdas.cpp)
target_compile_options(lambdas PRIVATE -Wall -Wextra -pedantic)
