#include "socket.h"

#include <errno.h>
#include <syslog.h>
#include <string.h>

#define LISTENQ 10

namespace lib {
namespace sock {

Sockaddr::Sockaddr(const std::string& port, sa_family_t family) {
  init(port, family);
}

void Sockaddr::init(const std::string& port, sa_family_t family) {
  this->port = port;
  if (family == AF_LOCAL) {
    syslog(LOG_INFO, "Creating Unix Domain socket address: %s", port.c_str());
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = family;
    memmove(addr.sun_path, port.c_str(), port.size());
    memmove(&sockaddr_, (struct sockaddr_storage *)&addr, sizeof(addr));
    return;
  }
  syslog(LOG_INFO, "Creating socket address on port: %s", port.c_str());
  int status;
  struct addrinfo hints;
  struct addrinfo *addrinfos;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = family;
  status = getaddrinfo(nullptr, port.c_str(), &hints, &addrinfos);
  if (status != 0) {
    syslog(LOG_INFO, "Failed to create socket address: %s", gai_strerror(status));
  }
  memmove(&sockaddr_, (struct sockaddr_storage *)addrinfos->ai_addr,
      sizeof(sockaddr_));
  freeaddrinfo(addrinfos);
}

struct sockaddr *Sockaddr::sockaddr() const {
  return (struct sockaddr *)(&sockaddr_);
}

socklen_t Sockaddr::size() const {
  if (sockaddr_.ss_family == AF_LOCAL) {
    return SUN_LEN((struct sockaddr_un *)&sockaddr_);
  }
  return sizeof(sockaddr_);
}

Socket::Socket() {
  if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {
    throw SocketError("Failed to create socket");
  }
  int y = 1;
  if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) == -1) {
    throw SocketError("Failed to make socket address reusuable");
  }
  state_ = SocketState::INITIALIZED;
}

Socket::Socket(int fd, const Sockaddr& local, const Sockaddr& remote) 
    : fd(fd), local_(local), remote_(remote) {
  state_ = SocketState::CONNECTED;
}

void Socket::bind_(const std::string& port) {
  if (state_ != SocketState::INITIALIZED) {
    throw SocketError("Failed to bind socket. Invalid socket state");
  }
  local_.init(port, AF_INET6);
  if (bind(fd, local_.sockaddr(), local_.size()) == -1) {
    throw SocketError("Failed to bind socket to port" + port);
  }
  state_ = SocketState::BOUND;
}

void Socket::listen_() {
  if (state_ != SocketState::BOUND) {
    throw SocketError("Failed to listen on socket. Invalid socket state");
  }
  if (listen(fd, LISTENQ) == -1) {
    throw SocketError("Failed to listen on socket" + fd);
  }
  state_ = SocketState::LISTENING;
}

std::unique_ptr<Socket> Socket::accept_() {
  if (state_ != SocketState::LISTENING) {
    throw SocketError("Failed to accept on socket. Invalid socket state");
  }
  remote_.init(local_.port, AF_INET6);
  socklen_t remote_len = remote_.size();
  int connfd;
  if ((connfd = accept(fd, remote_.sockaddr(), &remote_len)) == -1) {
    throw SocketError("Failed to accept on socket" + fd);
  }
  return std::make_unique<Socket>(connfd, remote_, local_);
}

void Socket::writen(const char *msg, size_t n) {
  ssize_t written;
  size_t nleft = n;
  const char *loc;
  loc = (const char *)msg;

  while (nleft > 0) {
    if ((written = write(fd, loc, nleft)) <= 0) {
      if (errno != EINTR) {
        syslog(LOG_ERR, "Write error; %m");
        throw SocketError("Failed while writing to socket");
      }
    }
    nleft -= written;
    loc += written;
  }
}

void Socket::write_(const std::string& msg) {
  writen(msg.c_str(), msg.size());
}

int Socket::read_(std::string& msg) {
  ssize_t nread;
  ssize_t n;
  bool keep_reading = true;
  while (keep_reading) {
    n = read(fd, buffer_, MAXLINE);
    if (n < 0) {
      if (errno != EINTR) {
        syslog(LOG_ERR, "Read error; %m");
        throw SocketError("Failed while reading from socket");
      }
    }
    nread += n;
    msg += buffer_;
    keep_reading = false;
  }
  return nread;
}

Socket::~Socket() {
  if (state_ != SocketState::UNINITIALIZED) {
    close(fd);
  }
}

} // socket
} // lib
