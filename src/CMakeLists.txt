find_package(Threads REQUIRED)

add_library(NekosBest)

target_sources(NekosBest PRIVATE
    categories.c client.c fetch.c info.c response.c search.c httpclient.c
)

target_include_directories(NekosBest PUBLIC ../include)

if (MSVC)
    target_compile_definitions(NekosBest PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif ()

if (FALSE)
    find_package(CURL REQUIRED)
    target_link_libraries(NekosBest PUBLIC ${CURL_LIBRARIES})
    target_sources(NekosBest PRIVATE httpclient_libcurl.c)
else ()
    target_link_libraries(NekosBest PRIVATE winhttp shlwapi)
    target_sources(NekosBest PRIVATE httpclient_winhttp.c)
endif ()

# dependencies

target_sources(NekosBest PRIVATE
    ../dev-deps/parson/parson.c
    ../dev-deps/tinycthread/source/tinycthread.c
)

target_include_directories(NekosBest PRIVATE
    ../dev-deps/tinycthread/source
    ../dev-deps/parson
    ${NB_VERSION_HEADER_DIR}
)

target_link_libraries(NekosBest PRIVATE Threads::Threads)