#pragma once

#include "socket.h"

#include <syslog.h>

namespace lib {
namespace server {

template <class Handler>
class BlockingServer {
 public:
  BlockingServer(const std::string& port) {
    socket = std::make_unique<lib::sock::Socket>(port);
    socket->Bind();
    socket->Listen();
  }
  virtual ~BlockingServer() {}
  void serve() {
    for ( ; ; ) {
      socket->Accept();
      int pid = fork();
      if (pid < 0) {
        // error
        syslog(LOG_WARNING, "failed to fork child to handle connection");
      } else if (pid == 0) {
        // child
        close(socket->getListenFd());
        std::string msg;
        socket->Read(&msg);
        socket->Write(handler().handle(msg));
        exit(0);
      }
      else {
        // parent
        close(socket->getConnFd());
      }
    }
  }
  std::unique_ptr<lib::sock::Socket> socket;
 private:
  Handler& handler() { return *static_cast<Handler*>(this); }
};

} // namespace server
} // namespace lib
