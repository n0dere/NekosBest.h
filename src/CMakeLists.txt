configure_file(./version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(NekosBest nekosbest.c httpclient_core.c httpclient_curl.c)

if (MSVC)
    target_compile_definitions(NekosBest PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(CURL REQUIRED)

target_link_libraries(NekosBest PUBLIC ${CURL_LIBRARIES})

target_include_directories(NekosBest PRIVATE ../include)

target_link_libraries(NekosBest PRIVATE winhttp shlwapi)