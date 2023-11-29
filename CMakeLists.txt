cmake_minimum_required(VERSION 3.24)
project(Vypr VERSION 1.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Vypr)
add_subdirectory(VyprTest)
