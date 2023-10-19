add_library(NekosBest_HTTP STATIC httpclient_core.c)

target_include_directories(NekosBest_HTTP PUBLIC .)

if (TRUE)
    find_package(CURL REQUIRED)
    target_link_libraries(NekosBest_HTTP PUBLIC ${CURL_LIBRARIES})
    target_sources(NekosBest_HTTP PRIVATE httpclient_curl.c)
else ()
    target_link_libraries(NekosBest PRIVATE winhttp shlwapi)
    target_sources(NekosBest_HTTP PRIVATE httpclient_win.c)
endif ()

target_include_directories(NekosBest_HTTP PRIVATE
    ../include ${NB_VERSION_HEADER_DIR}
)