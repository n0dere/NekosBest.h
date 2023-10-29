find_package(Threads REQUIRED)

add_library(NekosBest
    categories.c client.c fetch.c info.c response.c search.c
    httpclient.c download.c
    ${PROJECT_SOURCE_DIR}/dev-deps/parson.c
    ${PROJECT_SOURCE_DIR}/dev-deps/tinycthread.c
)

target_link_libraries(NekosBest PRIVATE Threads::Threads)

target_compile_features(NekosBest PUBLIC c_std_11)

if (MSVC)
    target_compile_definitions(NekosBest PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

if (NB_WINHTTP AND WIN32)
    target_link_libraries(NekosBest PUBLIC winhttp shlwapi)
    target_sources(NekosBest PRIVATE httpclient_winhttp.c)
else ()
    find_package(CURL REQUIRED)
    target_link_libraries(NekosBest PUBLIC ${CURL_LIBRARIES})
    target_sources(NekosBest PRIVATE httpclient_libcurl.c)
endif ()

set_target_properties(NekosBest PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    DEFINE_SYMBOL _NB_SHARED
)

if (BUILD_SHARED_LIBS AND WIN32)
    if (MINGW)
        target_link_libraries(NekosBest PRIVATE "-static")
    endif ()

    set_target_properties(NekosBest PROPERTIES PREFIX "")
    target_compile_definitions(NekosBest INTERFACE _NB_DLL_EXPORT)
endif ()

target_include_directories(NekosBest PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(NekosBest PRIVATE
    ${PROJECT_SOURCE_DIR}/dev-deps ${CMAKE_CURRENT_LIST_DIR}
)

configure_file(version.h.in ${CMAKE_CURRENT_LIST_DIR}/version.h)

if (NB_INSTALL)
    install(TARGETS NekosBest EXPORT NekosBestTargets
            DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()