#include <string>

namespace ssfs {

class Fd {
  Fd();
  Fd(int infd);
  ~Fd() noexcept;
  Fd(const Fd& other);
  Fd(Fd&& other) noexcept;
  Fd& operator=(const Fd& other); 
  Fd& operator=(const Fd&& other);
  Fd& dup2(Fd&& other);
  static Fd dup(const Fd& other);
  int fd() const;
  operator int() const;
 private:
  int fd;
};

} // namespace ssfs
