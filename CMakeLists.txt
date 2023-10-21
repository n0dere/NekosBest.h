cmake_minimum_required(VERSION 3.10)
project(NekosBest VERSION 2.0.0)

set(NB_VERSION_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/include CACHE STRING "")

configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/version.h.in
    ${NB_VERSION_HEADER_DIR}/version.h
)

add_subdirectory(src)

add_executable(test main.c)
target_link_libraries(test PRIVATE NekosBest)