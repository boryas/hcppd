#pragma once

#include <dirent.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace lib {
namespace fs {

class Stat {
 public:
  Stat(const std::string& path);
  bool dir;
};

template <class T>
class File {
 public:
  File() {}
  virtual ~File() {}
  void read() {
    f().open();
    while (f().readNext()) {
      continue;
    }
  }
 private:
  T& f() { return *static_cast<T*>(this); }
};

class Directory : public File<Directory> {
 public:
  Directory(const std::string& path);
  virtual ~Directory();
  std::string path;
  void open();
  bool readNext();
  std::vector<std::string> children;
 private:
  DIR* dir_;
};

class TextFile : public File<TextFile> {
 public:
  TextFile(const std::string& path);
  virtual ~TextFile() = default;
  std::string path;
  void open();
  bool readNext();
  std::vector<std::string> lines;
};

} // namespace fs
} // namespace lib
