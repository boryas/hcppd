#include "Fd.h"
#include <exception>
#include <sstream>

namespace ssfs {

Fd::Fd() : fd(-1) {}

Fd::Fd(int infd) : fd(infd) {
  if (infd < 0) {
    std::stringstream msg;
    msg << "Bad fd passed into Fd: " << infd;
    throw std::runtime_error(msg.str());
  }
}

Fd::~Fd() noexcept {
  // LIKELY/UNLIKELY?!?!?!
  if (fd > 0) {
    // if close fails, register it in a global lib "keep tryna cleanup" zone?
    close(fd);
  }
}

Fd::Fd(const Fd& other) {
  *this == other;
}

Fd::Fd(Fd&& other) noexcept {
  *this == std::move(other);
}

Fd& Fd::operator=(const Fd& other) {
  fd = ::dup(other.fd());
  if (fd == -1) {
    // print some nasty
    std::stringstream msg;
    msg << "Failed to dup " << other.fd();
    throw std::runtime_error(msg.str());
  }
}

Fd& operator=(Fd&& other) noexcept {
  fd = other.fd;
  other.fd = -2;
}

Fd& Fd::dup2(Fd&& other) {
  ::dup2(fd, other.fd());
  return *this;
}

static Fd dup(const Fd& other) {
  int fd = ::dup(other.fd());
  return Fd{fd};
}

int fd() const {
  return fd;
}

operator int() const {
  return fd();
}

} // namespace ssfs
