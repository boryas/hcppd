#include "socket.h"

#define SERV_PORT 8080
#define LISTENQ 10

namespace bostack {
namespace sock {

Sockaddr::Sockaddr() {
  memset(&sockaddr_, 0, sizeof(sockaddr_));
  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr_.sin_port = htons(SERV_PORT);
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
    std::cerr << "Failed to create socket " << strerror(errno) << std::endl;
  }
  int y = 1;
  if ((setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) == -1) {
    sockerr_ = errno;
    std::cerr << "Failed to make socket address reusable "
              << strerror(errno) << std::endl;
  }
}

Socket::~Socket() {
  close(listenfd_);
}

int Socket::Bind() {
  if (bind(listenfd_, servaddr_.sockaddr(), servaddr_.size()) == -1) {
    sockerr_ = errno;
    std::cerr << "Failed to bind socket " << listenfd_
              << " error was: " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

int Socket::Listen() {
  if (listen(listenfd_, LISTENQ) == -1) {
    sockerr_ = errno;
    std::cerr << "Failed to listen on socket " << listenfd_
              << " error was: " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

int Socket::Accept(socklen_t *clilen) {
  *clilen = cliaddr_.size();
  if ((connfd_ = accept(listenfd_, cliaddr_.sockaddr(), clilen)) == -1) {
    sockerr_ = errno;
    std::cerr << "Failed to accept on socket " << listenfd_
              << " error was: " << strerror(errno) << std::endl;
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
        std::cerr << "Write error" << std::endl;
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
        std::cerr << "Read error" << std::endl;
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
}
