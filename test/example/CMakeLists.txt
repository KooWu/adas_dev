cmake_minimum_required (VERSION 3.16)

project(example)

add_executable(example test.cpp)

install(TARGETS example RUNTIME DESTINATION bin)