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

void Directory::read() {
  errno = 0;
  struct dirent* d;
  while ((d = readdir(dir_))) {
    std::string p(d->d_name);
    if (p == "." || p == "..") {
      continue;
    }
    contents.push_back(p);
  }
  if (errno != 0) {
    throw FsError();
  }
}

File::File(const std::string& path) : path(path) {
  if ((file_ = fopen(path.c_str(), "r")) == NULL) {
    syslog(LOG_ERR, "Error opening file %s: %m", path.c_str());
    if (errno == ENOENT) {
      throw PathNotFoundError();
    } else {
      throw FsError();
    }
  }
};

File::~File() {
  fclose(file_);
};

void File::read() {
  errno = 0;
  char *line = NULL;
  size_t n = 0;
  while((getline(&line, &n, file_)) != -1) {
    lines.emplace_back(line);
  }
  free(line);
  if (errno != 0) {
    throw FsError();
  }
}

} // namespace fs
} // namespace lib
