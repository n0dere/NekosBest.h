link_libraries(NekosBest)

if (MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()

add_executable(download download.c)
add_executable(search_and_download search_and_download.c)
add_executable(simple_fetch simple_fetch.c)
add_executable(simple_search simple_search.c)