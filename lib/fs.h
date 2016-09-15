namespace lib {
namespace fs {

class Directory {
 public:
  Directory(const std::string& path);
  std::string& path;
}

class File {
 public:
  File(const std::string& path);
  std::string& path;
}

} // namespace fs
} // namespace lib
