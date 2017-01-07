#include "fd.h"

#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include <sstream>

namespace lib {
namespace fd {

Fd::Fd(const int fd) {
  errno = 0;
  if (fcntl(fd, F_GETFD) == -1) {
    if (errno == EBADF) {
      std::stringstream ss;
      ss << "Invalid negative fd" << fd;
      throw FdError(ss.str());
    }
  }
  this->fd = fd;
}

Fd::~Fd() {
  if (fd > 0) {
    close(fd);
  }
}

void Fd::writen(const char *msg, size_t n) {
  ssize_t written;
  size_t nleft = n;
  const char *loc;
  loc = (const char *)msg;

  while (nleft > 0) {
    if ((written = write(fd, loc, nleft)) <= 0) {
      if (errno != EINTR) {
        syslog(LOG_ERR, "Write error; %m");
        throw FdError("Failed while writing to fd " + fd);
      }
    }
    nleft -= written;
    loc += written;
  }
}

// buf must have capacity >= n
size_t Fd::readn(std::string& buf, size_t n) {
  size_t left = n;
  ssize_t nread;
  char *ptr = &buf[0];
  while (left > 0) {
    nread = read(fd, ptr, left);
    if (nread < 0) {
      if (errno != EINTR) {
        syslog(LOG_ERR, "Read error; %m");
        throw FdError("Failed while reading from fd " + fd);
      }
      nread = 0;
    }
    left -= nread;
    ptr += nread;
    // HACK until we have non-blocking mode
    // if the other end never closes but sends less than n, we block forever
    return n - left;
  }
  return n - left;
}

void Fd::write_(const std::string& msg) {
  writen(msg.c_str(), msg.size());
}

} // namespace fd
} // namespace lib
