cmake_minimum_required(VERSION 3.10)

project(storage-datastructure)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(storage
                ${CMAKE_SOURCE_DIR}/main.cpp
                ${CMAKE_SOURCE_DIR}/skiplist/skiplist.cpp)

include_directories(${CMAKE_SOURCE_DIR}/skiplist)
