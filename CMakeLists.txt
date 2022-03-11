cmake_minimum_required(VERSION 3.10)

project(std-expected LANGUAGES CXX)

add_library (std-expected INTERFACE)

target_include_directories(std-expected INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
