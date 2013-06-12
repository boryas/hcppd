#include "socket.h"

#define LISTENQ 10

namespace sock {

Sockaddr::Sockaddr() {
  memset(&sockaddr_, 0, sizeof(sockaddr_));
  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
}

void Sockaddr::init(int port) {
  sockaddr_.sin_port = htons(port);
}

struct sockaddr *Sockaddr::sockaddr() {
  return (struct sockaddr *)(&sockaddr_);
}

socklen_t Sockaddr::size() {
  return sizeof(sockaddr_);
}

Socket::Socket() {
  if ((listenfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to create socket %m");
  }
  int y = 1;
  if ((setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to make socket address reusable %m");
  }
}

Socket::~Socket() {
  close(listenfd_);
}

int Socket::Bind(int port) {
  servaddr_.init(port);
  if (bind(listenfd_, servaddr_.sockaddr(), servaddr_.size()) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to bind socket %d; %m", listenfd_);
    return -1;
  }
  return 0;
}

int Socket::Listen() {
  if (listen(listenfd_, LISTENQ) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to listen on socket %d; %m", listenfd_);
    return -1;
  }
  return 0;
}

int Socket::Accept(socklen_t *clilen) {
  *clilen = cliaddr_.size();
  if ((connfd_ = accept(listenfd_, cliaddr_.sockaddr(), clilen)) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to accept on socket %d; %m", listenfd_);
    return -1;
  }
  return 0;
}

int Socket::Writen(const char *msg, size_t n) {
  ssize_t written;
  size_t nleft = n;
  int sockerr;
  const char *loc;
  loc = (const char *)msg;

  while (nleft > 0) {
    if ((written = write(connfd_, loc, nleft)) <= 0) {
      sockerr = errno;
      if (sockerr != EINTR) {
        sockerr_ = sockerr;
        syslog(LOG_ERR, "Write error; %m");
        return -1;
      }
    }
    nleft -= written;
    loc += written;
  }
  return 0;
}

int Socket::Write(const std::string& msg) {
  return Writen(msg.c_str(), msg.size());
}

int Socket::Read(std::string *msg) {
  ssize_t nread;
  ssize_t n;
  int sockerr;
  bool keep_reading = true;
  while (keep_reading) {
    n = read(connfd_, buffer_, MAXLINE);
    if (n < 0) {
      sockerr = errno;
      if (sockerr != EINTR) {
        sockerr_ = sockerr;
        syslog(LOG_ERR, "Read error; %m");
        return -1;
      }
    }
    nread += n;
    *msg += buffer_;
    keep_reading = false;
  }
  return nread;
}

}
