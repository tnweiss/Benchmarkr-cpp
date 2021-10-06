if (MSVC)
    ####### Warnings ##########
    # Disables
    #    warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
    #    To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS )

endif()
