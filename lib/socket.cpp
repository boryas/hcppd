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
  int fd;
  if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {
    throw SocketError("Failed to create socket");
  }
  fd_ = std::make_unique<lib::fd::Fd>(fd);
  int y = 1;
  if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) == -1) {
    throw SocketError("Failed to make socket address reusuable");
  }
  state_ = SocketState::INITIALIZED;
}

Socket::Socket(std::unique_ptr<lib::fd::Fd> fd,
               const Sockaddr& local,
               const Sockaddr& remote)
    : fd_(std::move(fd)), local_(local), remote_(remote) {
  state_ = SocketState::CONNECTED;
}

void Socket::bind_(const std::string& port) {
  if (state_ != SocketState::INITIALIZED) {
    throw SocketError("Failed to bind socket. Invalid socket state");
  }
  local_.init(port, AF_INET6);
  if (bind(fd_->fd, local_.sockaddr(), local_.size()) == -1) {
    throw SocketError("Failed to bind socket to port" + port);
  }
  state_ = SocketState::BOUND;
}

void Socket::listen_() {
  if (state_ != SocketState::BOUND) {
    throw SocketError("Failed to listen on socket. Invalid socket state");
  }
  if (listen(fd_->fd, LISTENQ) == -1) {
    throw SocketError("Failed to listen on socket" + fd_->fd);
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
  if ((connfd = accept(fd_->fd, remote_.sockaddr(), &remote_len)) == -1) {
    throw SocketError("Failed to accept on socket" + fd_->fd);
  }
  return std::make_unique<Socket>(
      std::make_unique<lib::fd::Fd>(connfd), remote_, local_);
}

void Socket::write_(const std::string& msg) {
  fd_->write_(msg);
}

int Socket::readn(std::string& buf, int n) {
  return fd_->readn(buf, n);
}

} // socket
} // lib
