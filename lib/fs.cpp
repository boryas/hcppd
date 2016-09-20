#include "fs.h"

#include <syslog.h>
#include <sys/stat.h>

#include <sstream>

namespace lib {
namespace fs {

Stat::Stat(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == -1) {
    if (errno == ENOENT) {
      syslog(LOG_ERR, "path not found: %s: %m", path.c_str());
      return;
    }
  }
  if (st.st_mode & S_IFDIR) {
    dir = true;
  } else {
    dir = false;
  }
}

Directory::Directory(const std::string& path) : path(path) {};

Directory::~Directory() {
  if (dir_) {
    closedir(dir_);
  }
}

void Directory::open() {
  DIR* d;
  if ((d = opendir(path.c_str())) == NULL) {
    syslog(LOG_ERR, "Error opening directory %s: %m", path.c_str());
    return;
  }
  dir_ = d;
}

bool Directory::readNext() {
  if (!dir_) {
    return false;
  }
  struct dirent* d = readdir(dir_);
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

TextFile::TextFile(const std::string& path) : path(path) {};

void TextFile::open() {
}

bool TextFile::readNext() {
  std::string line;
  return true;
}

} // namespace fs
} // namespace lib
