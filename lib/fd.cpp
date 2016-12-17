#include "fd.h"

#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include <sstream>

#define MAXLINE 4096

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

int Fd::read_(std::string& msg) {
  char buffer_[MAXLINE];
  ssize_t nread;
  ssize_t n;
  bool keep_reading = true;
  while (keep_reading) {
    n = read(fd, buffer_, MAXLINE);
    if (n < 0) {
      if (errno != EINTR) {
        syslog(LOG_ERR, "Read error; %m");
        throw FdError("Failed while reading from fd " + fd);
      }
    }
    nread += n;
    msg += buffer_;
    keep_reading = false;
  }
  return nread;
}

void Fd::write_(const std::string& msg) {
  writen(msg.c_str(), msg.size());
}

} // namespace fd
} // namespace lib
