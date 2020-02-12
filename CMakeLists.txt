cmake_minimum_required (VERSION 2.6)
project (Transport)

# The version number.
set (Transport_VERSION_MAJOR 1)
set (Transport_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 17)

include_directories (src)
include_directories (include)

add_library (Transport
    include/Transport.h
    src/Transport.hpp
    src/Transport.cpp
)
target_link_libraries (Transport Tools)
target_include_directories(Transport PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory (example)
