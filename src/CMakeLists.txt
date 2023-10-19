file(GLOB NB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(NekosBest SHARED ${NB_SOURCES})

target_link_libraries(NekosBest PRIVATE NekosBest_HTTP)

target_include_directories(NekosBest PUBLIC ../include)
target_include_directories(NekosBest PRIVATE ${NB_VERSION_HEADER_DIR})