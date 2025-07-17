cmake_minimum_required(VERSION 3.10)
project(his LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")

add_executable(his main.cpp his.cpp)

target_link_libraries(his PRIVATE ncursesw tinfo)

install(TARGETS his DESTINATION bin)
