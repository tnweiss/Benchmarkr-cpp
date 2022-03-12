####### Setup configuration ##########
set(CONFIG_REMOTE https://raw.githubusercontent.com/tnweiss/benchmarkr-configuration/master)
set(CONFIG_DIR ${CMAKE_SOURCE_DIR}/dependencies/setup)

set(REMOTE_CONFIG_HEADER ${CMAKE_SOURCE_DIR}/include/benchmarkr-common/resolver/remote_config.h)
set(REMOTE_CONFIG_SOURCE ${CMAKE_SOURCE_DIR}/src/benchmarkr-common/resolver/remote_config.cpp)

####### fetch all setup resources ##########
if (NOT EXISTS ${REMOTE_CONFIG_HEADER} OR NOT EXISTS ${REMOTE_CONFIG_SOURCE})
    file(DOWNLOAD ${CONFIG_REMOTE}/resources.txt ${CONFIG_DIR}/resources.txt)
    file(READ ${CONFIG_DIR}/resources.txt resources)
    string(REGEX REPLACE "\n" ";" resources "${resources}")

    ####### variables holding c++ contents ##########
    set(output_h "
#include <map>
#include <string>

namespace benchmarkr {

/**
 * Get the contents of a file stored on the remote config
 *   repository. This does not load a file from the
 *   local filesystem
 * @param filename the name of the file to retrieve
 * @return the contents of the file
 */
std::map<std::string, std::string>& get_remote_files()\;

}
")
    set(output_cpp "
#include \"benchmarkr-common/resolver/remote_config.h\"

std::map<std::string, std::string>& benchmarkr::get_remote_files() {
  static std::map<std::string, std::string> files\;

  if (files.empty()) {
    files = {
      { \"\", \"\" }")

    ####### for each file in the list create a variable ##########
    message("\nAuto Generating configuration file function (src/benchmarkr/commands/init/remote_config.h/.cpp)")
    foreach(fileName ${resources})
        message("  adding file ${fileName}...")
        ####### download the file ##########
        file(DOWNLOAD ${CONFIG_REMOTE}/${fileName} ${CONFIG_DIR}/${fileName})
        message("    downloaded")

        ####### read the contents ##########
        file(READ ${CONFIG_DIR}/${fileName} resource)
        message("    read")

        ####### create a variable for the data ##########
        set(output_cpp "${output_cpp},
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        { \"${fileName}\", ")
        message("    generated")

        set(i 0)
        string(LENGTH ${resource} RESOURCE_LEN)
        while(${i} LESS ${RESOURCE_LEN})
            string(SUBSTRING ${resource} ${i} 100 t)
            MATH(EXPR i "${i}+100")
            set(output_cpp "${output_cpp}
            R\"(${t})\"")
            message(${t})



        endwhile()

        set(output_cpp "${output_cpp} }")


    endforeach()

    set(output_cpp "${output_cpp}
    }\;
  }

  return files\;
}
")
    message("")

    ####### write the contents to the c++/h files ##########
    file(WRITE ${REMOTE_CONFIG_HEADER} ${output_h})
    file(WRITE ${REMOTE_CONFIG_SOURCE} ${output_cpp})

endif()
