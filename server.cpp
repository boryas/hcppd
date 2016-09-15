#include "server.h"

#include <sys/types.h>
#include <unistd.h>

namespace hcppd {

std::string HttpServer::handleRequest(const lib::http::HttpRequest& request) {
  return request.uri();
}

std::string HttpServer::handleConnection() {
  std::string msg;
  sock_->Read(&msg);
  lib::http::HttpRequest req(msg);
  syslog(LOG_INFO, req.request_line.uri.c_str());
  return handleRequest(req);
}

void HttpServer::serve() {
  syslog(LOG_INFO, "Server listening on %s", port.c_str());
  sock_.reset(new lib::sock::Socket(port));
  sock_->Bind();
  sock_->Listen();
  for ( ; ; ) {
    sock_->Accept();
    int pid = fork();
    if (pid < 0) {
      // error
      syslog(LOG_WARNING, "failed to fork child to handle connection");
    } else if (pid == 0) {
      // child
      close(sock_->getListenFd());
      sock_->Write(handleConnection());
      exit(0);
    }
    else {
      // parent
      close(sock_->getConnFd());
    }
  }
}
} //namespace hcppd
