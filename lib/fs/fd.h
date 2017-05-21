#pragma once

#include <errno.h>
#include <stdexcept>

namespace ssfs {
namespace fs {

// Class that manages an open file descriptor.
// Provides typical file operations like read and write.
// Closes the fd on destruction.
// Typical use case would be to construct it around some function that
// returns a file descriptor.
// e.g. std::make_unique<Fd>(open(..))
class Fd {
 public:
  Fd(int fd);
  virtual ~Fd();
  void write_(const std::string& msg);
  size_t readn(std::string& buf, size_t n);
  int fd = -1;
 private:
  void writen(const char *msg, size_t n);
};

class FdError : public std::runtime_error {
 public:
  FdError(const std::string& reason)
    : std::runtime_error("FdError: " + reason) {}
};

} // namespace fs
} // namespace ssfs
