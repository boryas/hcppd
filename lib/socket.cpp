#include "socket.h"

#include <syslog.h>

#define LISTENQ 10

namespace lib {
namespace sock {

Sockaddr::Sockaddr(const std::string& service, sa_family_t family) {
  if (family == AF_LOCAL) {
    syslog(LOG_INFO, "Creating Unix Domain socket address: %s", service.c_str());
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = family;
    memmove(addr.sun_path, service.c_str(), service.size());
    memmove(&sockaddr_, (struct sockaddr_storage *)&addr, sizeof(addr));
    return;
  }
  syslog(LOG_INFO, "Creating socket address on port: %s", service.c_str());
  int status;
  struct addrinfo hints;
  struct addrinfo *addrinfos;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = family;
  status = getaddrinfo(nullptr, service.c_str(), &hints, &addrinfos);
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


Socket::Socket(const std::string& service) {
  service_ = service;
  family_ = AF_INET6;
}

Socket::Socket(const std::string& service, sa_family_t family) {
  service_ = service;
  family_ = family;
}

Socket::~Socket() {
  close(listenfd_);
}

int Socket::Bind() {
  if ((listenfd_ = socket(family_, SOCK_STREAM, 0)) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to create socket %m");
  }
  int y = 1;
  if ((setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y))) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to make socket address reusable %m");
  }
  servaddr_ = std::make_unique<Sockaddr>(service_, family_);
  if (bind(listenfd_, servaddr_->sockaddr(), servaddr_->size()) == -1) {
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
  cliaddr_ = std::make_unique<Sockaddr>(service_, family_);
  return 0;
}

int Socket::Accept() {
  clilen_ = cliaddr_->size();
  if ((connfd_ = accept(listenfd_, cliaddr_->sockaddr(), &clilen_)) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to accept on socket %d; %m", listenfd_);
    return -1;
  }
  return 0;
}

int Socket::Connect(const Sockaddr& addr) {
  if ((connfd_ = socket(addr.sockaddr()->sa_family, SOCK_STREAM, 0)) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to create socket %m");
  }

  if (connect(connfd_, addr.sockaddr(), addr.size()) == -1) {
    sockerr_ = errno;
    syslog(LOG_ERR, "Failed to connect on socket %d; %m", listenfd_);
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

} // socket
} // lib
