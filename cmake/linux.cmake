if (UNIX)
    
    ####### Thread libraries ##########
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wno-deprecated-declarations")

    link_directories(/usr/local/lib)
endif()