#include "fs.h"

#include <errno.h>
#include <string.h>

#include <sstream>

namespace lib {
namespace fs {

Stat::Stat(const std::string& path) {
  struct stat st;
  if (stat(path.c_str(), &st) == -1) {
    if (errno == ENOENT) {
      throw PathNotFoundError(path);
    } else {
      throw FsError("Failed to stat " + path + ": " + strerror(errno));
    }
  }
  mode_ = st.st_mode;
}

Directory::Directory(const std::string& path) : path(path) {
  if ((dir_ = opendir(path.c_str())) == NULL) {
    if (errno == ENOENT) {
      throw PathNotFoundError(path);
    } else {
      throw FsError("Failed to open directory: " + path +
          ": " + strerror(errno));
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
    throw FsError("Failed to read contents of directory " + path +
        ": " + strerror(errno));
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
    ss << line << "\n";
  }
  return ss.str();
}

std::string readFile(const std::string& path) {
  Stat s(path);
  if (!s.isRegularFile()) {
    throw FsError("Failed to read file " + path + 
        ": it is not a regular file");
  }
  File f(path);
  return f.read();
}

} // namespace fs
} // namespace lib
