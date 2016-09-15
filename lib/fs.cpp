#include <dirent.h>
#include <syslog.h>
#include <sys/stat.h>

#include <fstream>
#include <sstream>

namespace lib {
namespace fs {

Directory::Directory(const std::string& path) : path(path) {}

File::File(const std::string& path) : path(path) {}

} // namespace fs
} // namespace lib
