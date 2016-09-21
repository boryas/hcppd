#pragma once

#include <dirent.h>
#include <stdio.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace lib {
namespace fs {

class FsError : public std::runtime_error {
 public:
  FsError() : std::runtime_error("FsError") {}
};

class PathNotFoundError : public FsError {
 public:
  PathNotFoundError() {}
};

class Stat {
 public:
  Stat(const std::string& path);
  bool dir;
};

class Directory {
 public:
  Directory(const std::string& path);
  virtual ~Directory();
  std::string path;
  void read();
  std::vector<std::string> contents;
 private:
  DIR* dir_;
};

class File {
 public:
  File(const std::string& path);
  virtual ~File();
  std::string path;
  void read();
  std::vector<std::string> lines;
 private:
  FILE* file_;
};

} // namespace fs
} // namespace lib
