#include <string>

namespace ssfs {

class Fd {
 public:
  Fd();
  Fd(int infd);
  ~Fd() noexcept;
  Fd(const Fd& other);
  Fd(Fd&& other) noexcept;
  Fd& operator=(const Fd& other); 
  Fd& operator=(Fd&& other);
  Fd& dup2(Fd&& other);
  int fd() const;
  operator int() const;
  static Fd dup(const Fd& other);
 private:
  int fd_;
};

} // namespace ssfs
