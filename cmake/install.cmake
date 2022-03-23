########## Install directives if not a submodule ##########
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    # Explicitly define install targets
    install(CODE "set(CMAKE_INSTALL_LOCAL_ONLY true)")

    # Include cmake helpers for package config files
    # Follow this installation guideline: https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html
    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/cmake/benchmarkr/ConfigVersion.cmake"
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    configure_package_config_file(
            ${PROJECT_SOURCE_DIR}/cmake/benchmarkrConfig.cmake.in
            "${PROJECT_BINARY_DIR}/cmake/benchmarkrConfig.cmake"
        INSTALL_DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/benchmarkr-${PROJECT_VERSION}
    )

    install(FILES
            ${PROJECT_BINARY_DIR}/cmake/benchmarkrConfig.cmake
            ${PROJECT_BINARY_DIR}/cmake/benchmarkrConfigVersion.cmake
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/benchmarkr-${PROJECT_VERSION}
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/benchmarkr-dev
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            PATTERN *.h)
endif()

########## Install all Libraries ##########
install(TARGETS benchmarkrDev elkClient spdlog cpr
        EXPORT benchmarkrTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)