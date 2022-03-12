
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
std::map<std::string, std::string>& get_remote_files();

}
