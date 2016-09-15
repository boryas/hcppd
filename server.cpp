#include <sys/types.h>
#include <unistd.h>

#include "server.h"
#include "http.tab.hpp"

extern int yyparse();
extern int yylex_destroy();
extern int yy_scan_string(const char *str);
extern HttpRequest *request;

namespace hcppd {

std::string HttpServer::handleRequest(const HttpRequest& request) {
  std::unique_ptr<std::string> uri = std::move(request.request_line->uri);
  lib::sock::Socket dyn_client("/var/run/hcppd/sock", AF_LOCAL);
  lib::sock::Sockaddr addr("/var/run/hcppd/sock", AF_LOCAL);
  dyn_client.Connect(addr);
  dyn_client.Write(uri->c_str());
  std::string response;
  dyn_client.Read(&response);
  return response;
}

HttpRequest HttpServer::parseRequest(const std::string& requestString) {
  syslog(LOG_INFO, requestString.c_str());
  yy_scan_string(requestString.c_str());
  yyparse();
  yylex_destroy();
  syslog(LOG_INFO, "parsed");
  return std::move(*request);
}

std::string HttpServer::handleConnection() {
  std::string msg;
  sock_->Read(&msg);
  return handleRequest(parseRequest(msg));
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
