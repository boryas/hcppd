#include "fs.h"

#include <syslog.h>
#include <sys/stat.h>

#include <fstream>
#include <sstream>

namespace lib {
namespace fs {

Directory::Directory(const std::string path) : path(path) {};

void Directory::open() {
  DIR* d;
  if ((d = opendir(path.c_str())) == NULL) {
    syslog(LOG_ERR, "Error opening directory %s: %m", path.c_str());
    return;
  }
  dir_.reset(d);
}

bool Directory::readNext() {
  if (!dir_) {
    return false;
  }
  struct dirent* d = readdir(dir_.get());
  if (!d) {
    return false;
  }
  std::string p(d->d_name);
  if (p == "." || p == "..") {
    return true;;
  }
  children.push_back(p);
  return true;
}

File::File(const std::string path) : path(path) {};

void File::open() {
  fin_ = std::make_unique<std::ifstream>(path, std::ifstream::in);
}

bool File::readNext() {
  std::string line;
  if (!fin_) {
    return false;
  }
  auto b = getline(*fin_, line);
  if (!b) {
    return false;
  }
  lines.push_back(line);
  return true;
}

} // namespace fs
} // namespace lib
