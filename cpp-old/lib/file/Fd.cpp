#include "Fd.h"

#include <exception>
#include <sstream>

#include <unistd.h>

namespace ssfs {

Fd::Fd() : fd_(-1) {}

Fd::Fd(int fd) : fd_(fd) {
  if (fd_ < 0) {
    std::stringstream msg;
    msg << "Bad fd passed into Fd: " << fd_;
    throw std::runtime_error(msg.str());
  }
}

Fd::~Fd() noexcept {
  // LIKELY/UNLIKELY?!?!?!
  if (fd() > 0) {
    // if close fails, register it in a global lib "keep tryna cleanup" zone?
    ::close(fd_);
  }
}

Fd::Fd(const Fd& other) {
  *this = other;
}

Fd::Fd(Fd&& other) noexcept {
  *this = std::move(other);
}

Fd& Fd::operator=(const Fd& other) {
  fd_ = ::dup(other.fd());
  if (fd() == -1) {
    // print some nasty
    std::stringstream msg;
    msg << "Failed to dup " << other.fd();
    throw std::runtime_error(msg.str());
  }
  return *this;
}

Fd& Fd::operator=(Fd&& other) {
  fd_ = other.fd();
  other.fd_ = -2;
  return *this;
}

Fd& Fd::dup2(Fd&& other) {
  ::dup2(fd_, other.fd());
  return *this;
}

static Fd dup(const Fd& other) {
  int fd = ::dup(other.fd());
  return Fd{fd};
}

int Fd::fd() const {
  return fd_;
}

Fd::operator int() const {
  return fd();
}

} // namespace ssfs
