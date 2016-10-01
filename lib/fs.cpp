#include "fs.h"

#include <syslog.h>

#include <sstream>

namespace lib {
namespace fs {

Stat::Stat(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == -1) {
    if (errno == ENOENT) {
      syslog(LOG_ERR, "path not found: %s: %m", path.c_str());
      throw PathNotFoundError();
    } else {
      throw FsError();
    }
  }
  mode_ = st.st_mode;
}

Directory::Directory(const std::string& path) : path(path) {
  if ((dir_ = opendir(path.c_str())) == NULL) {
    syslog(LOG_ERR, "Error opening directory %s: %m", path.c_str());
    if (errno == ENOENT) {
      throw PathNotFoundError();
    } else {
      throw FsError();
    }
  }
};

Directory::~Directory() {
  closedir(dir_);
}

std::vector<std::string> Directory::contents() {
  std::vector<std::string> contents;
  errno = 0;
  struct dirent* d;
  while ((d = readdir(dir_))) {
    std::string p(d->d_name);
    if (p == "." || p == "..") {
      continue;
    }
    contents.emplace_back(std::move(p));
  }
  if (errno != 0) {
    throw FsError();
  }
  return contents;
}

File::File(const std::string& path) : path(path) {
  f_ = std::make_unique<std::ifstream>(path);
};

File::~File() {
};

std::vector<std::string> File::readLines() {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(*f_, line)) {
    lines.emplace_back(std::move(line));
  }
  return lines;
}

std::string File::read() {
  std::stringstream ss;
  std::string line;
  while (std::getline(*f_, line)) {
    ss << line;
  }
  return ss.str();
}

} // namespace fs
} // namespace lib
