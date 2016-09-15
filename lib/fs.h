#include <dirent.h>

#include <string>
#include <vector>

namespace lib {
namespace fs {

template <class T>
class File {
 public:
  void read() {
    f().open();
    while (f().readNext()) {
      continue;
    }
  }
 private:
  T& f() { return *static_cast<T*>(this); }
}

class Directory : File<Directory>{
 public:
  Directory(const std::string path);
  std::string& path;
  void open();
  bool readNext();
  std::vector<std::string> children;
 private:
  std::unique_ptr<DIR> dir_;
};

class File {
 public:
  File(const std::string path);
  std::string& path;
  void open();
  bool readNext();
  std::vector<std::string> lines;
 private:
  std::unique_ptr<std::ifstream> fin_;
};

} // namespace fs
} // namespace lib
