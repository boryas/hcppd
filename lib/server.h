#pragma once

#include "socket.h"

#include <syslog.h>

namespace lib {
namespace server {

template <class Handler>
class BlockingServer {
 public:
  BlockingServer(const std::string& port) {
    listen_sock = std::make_unique<lib::sock::Socket>();
    listen_sock->bind_(port);
    listen_sock->listen_();
  }
  virtual ~BlockingServer() {}
  void serve() {
    for ( ; ; ) {
      auto conn_sock = listen_sock->accept_();
      int pid = fork();
      if (pid < 0) {
        // error
        syslog(LOG_WARNING, "failed to fork child to handle connection");
      } else if (pid == 0) {
        // child
        listen_sock.reset();
        std::string buf;
        buf.resize(4096);
        conn_sock->readn(buf, 4096);
        conn_sock->write_(handler().handle(buf));
        exit(0);
      }
      else {
        // parent
        conn_sock.reset();
      }
    }
  }
  std::unique_ptr<lib::sock::Socket> listen_sock;
 private:
  Handler& handler() { return *static_cast<Handler*>(this); }
};

} // namespace server
} // namespace lib
