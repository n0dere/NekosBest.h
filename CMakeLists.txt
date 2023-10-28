cmake_minimum_required(VERSION 3.10)
project(NekosBest.h VERSION 2.0.0 LANGUAGES C)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" NB_STANDALONE)

include(GNUInstallDirs)
include(CMakeDependentOption)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(NB_BUILD_EXAMPLES "Build NekosBest.h examples" ${NB_STANDALONE})
option(NB_INSTALL "Generate installation target" ${NB_STANDALONE})

cmake_dependent_option(NB_WINHTTP "Use WINHTTP instead of libcurl" ON "WIN32" OFF)

add_subdirectory(src)

if (NB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (NB_INSTALL)
    install(FILES include/nekosbest.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()