cmake_minimum_required (VERSION 2.6)
project (TransportExample)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries (
    ${PROJECT_NAME}
LINK_PRIVATE
    Tools
    QueueTransport
)

