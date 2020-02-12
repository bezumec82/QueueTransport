cmake_minimum_required( VERSION 3.14 )

### Disable in-source build
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )
if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( SEND_ERROR "In-source builds are not allowed." )
endif ()

project (QueueTransport)
# The version number.
set( QueueTransport_VERSION_MAJOR 1 )
set( QueueTransport_VERSION_MINOR 0 )

########
### Misc
########
set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_COLOR_MAKEFILE ON )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17 -O0 -DLINUX_TERMINAL" )

set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_INSTALL_PREFIX           ${CMAKE_CURRENT_SOURCE_DIR}/out )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out )


###########
### Library
###########
message( "${CYN}Configuring library : ${PROJECT_NAME}${NOMR}" )
file( GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )

add_library(
    ${PROJECT_NAME}
STATIC
    ${CXX_FILES}
)

target_include_directories( 
    ${PROJECT_NAME}
PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
LINK_PRIVATE
    Tools
)

add_subdirectory( libs )
add_subdirectory( test )

