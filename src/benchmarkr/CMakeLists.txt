########## Benchmarkr Static Library ##########
add_executable(benchmarkr

        main.cpp
        help.cpp
        init.cpp
        upload.cpp
        command.cpp
        upload_watch.cpp
        init-context.cpp
)

# link to dependencies
target_link_libraries(benchmarkr ${BM_LL} PRIVATE benchmarkr-common)

# add precompiled headers
target_precompile_headers(benchmarkr PUBLIC benchmarkr_pch.h)
