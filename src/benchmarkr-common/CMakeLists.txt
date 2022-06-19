########## Benchmarkr Static Library ##########
add_library(benchmarkrCommon
    STATIC
        system.cpp
        system_lin.cpp
        system_win.cpp
        system_mac.cpp

        util.cpp
        util_lin.cpp
        util_win.cpp

        resolver/resolver.cpp
        resolver/cli_args.cpp
        resolver/remote_config.cpp
        resolver/default_resolver.cpp
        resolver/environment_resolver.cpp
        resolver/remote_config_resolver.cpp

        global_test_context.cpp
)

# create the alias
add_library(benchmarkrCommon::benchmarkrCommon ALIAS benchmarkrCommon)

# link to dependencies
target_link_libraries(benchmarkrCommon ${BM_COM_LL})

# add precompiled headers
target_precompile_headers(benchmarkrCommon PUBLIC benchmarkr_common_pch.h)
