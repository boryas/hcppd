#pragma once

#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace lib {
namespace fs {

class FsError : public std::runtime_error {
 public:
  FsError(const std::string& reason)
    : std::runtime_error("FsError: " + reason) {}
};

class PathNotFoundError : public FsError {
 public:
  PathNotFoundError(const std::string& path)
    : FsError("Path not found: " + path) {}
};

class Stat {
 public:
  Stat(const std::string& path);
  bool isDir() { return S_ISDIR(mode_); }
  bool isRegularFile() { return S_ISREG(mode_); }
 private:
  mode_t mode_;
};

class Directory {
 public:
  Directory(const std::string& path);
  virtual ~Directory();
  std::string path;
  std::vector<std::string> contents();
 private:
  DIR* dir_;
};

class File {
 public:
  File(const std::string& path);
  virtual ~File();
  std::string path;
  std::string read();
  std::vector<std::string> readLines();
 private:
  std::unique_ptr<std::ifstream> f_;
};

std::string readFile(const std::string& path);

} // namespace fs
} // namespace lib
