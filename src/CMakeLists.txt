find_package(Threads REQUIRED)

file(GLOB NB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(NekosBest SHARED ${NB_SOURCES})

target_include_directories(NekosBest PUBLIC ../include)

target_link_libraries(NekosBest PRIVATE NekosBest_HTTP)
target_include_directories(NekosBest PRIVATE ${NB_VERSION_HEADER_DIR})

# dependencies

target_sources(NekosBest PRIVATE
    ../dev-deps/parson/parson.c
    ../dev-deps/tinycthread/source/tinycthread.c
)

target_include_directories(NekosBest PRIVATE
    ../dev-deps/tinycthread/source
    ../dev-deps/parson
)

target_link_libraries(NekosBest PRIVATE Threads::Threads)