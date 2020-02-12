cmake_minimum_required (VERSION 2.6)
project (TransportExample)

set (CMAKE_CXX_STANDARD 17)

add_executable(TransportExample main.cpp)
target_link_libraries (TransportExample Tools)
target_link_libraries (TransportExample Transport)
target_link_libraries (TransportExample pthread)
