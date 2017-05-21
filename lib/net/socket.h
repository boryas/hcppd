#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <stdexcept>

#include "lib/fs/fd.h"

namespace ssfs {
namespace net {

class Sockaddr {
 public:
  Sockaddr() {};
  Sockaddr(const std::string& port, sa_family_t family);
  void init(const std::string& port, sa_family_t family);
  struct sockaddr *sockaddr() const;
  socklen_t size() const;
  std::string port;
 private:
  struct sockaddr_storage sockaddr_;
};

enum class SocketState {
  UNINITIALIZED,
  INITIALIZED,
  BOUND,
  LISTENING,
  CONNECTED,
};

class SocketError : public std::runtime_error {
 public:
  SocketError(const std::string& reason)
    : std::runtime_error("SocketError: " + reason) {}
};

class Socket {
 public:
  Socket();
  Socket(std::unique_ptr<ssfs::fs::Fd> fd,
         const Sockaddr& local,
         const Sockaddr& remote);
  void bind_(const std::string& port);
  void listen_();
  std::unique_ptr<Socket> accept_();
  void write_(const std::string& msg);
  int readn(std::string& buf, int n);
 private:
  std::unique_ptr<ssfs::fs::Fd> fd_;
  SocketState state_ = SocketState::UNINITIALIZED;
  Sockaddr local_;
  Sockaddr remote_;
};

} // namespace net
} // namespace ssfs
