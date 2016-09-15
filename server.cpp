#include "server.h"

#include <sys/types.h>
#include <unistd.h>

namespace hcppd {

std::string HttpServer::handleRequest(const lib::http::HttpRequest& request) {
  return request.uri();
  lib::sock::Socket dyn_client("/var/run/hcppd/sock", AF_LOCAL);
  lib::sock::Sockaddr addr("/var/run/hcppd/sock", AF_LOCAL);
  dyn_client.Connect(addr);
  dyn_client.Write(request.request_line.uri.c_str());
  std::string response;
  dyn_client.Read(&response);
  return response;
}

std::string HttpServer::handleConnection() {
  std::string msg;
  sock_->Read(&msg);
  lib::http::HttpRequest req(msg);
  syslog(LOG_INFO, req.request_line.uri.c_str());
  return handleRequest(req);
}

void startDynamicContentServer() {
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
