#include <sys/types.h>
#include <unistd.h>

#include "server.h"
#include "http.tab.hpp"

using namespace sock;
using namespace std;

extern int yyparse();
extern int yylex_destroy();
extern int yy_scan_string(const char *str);
extern HttpRequest *request;

namespace hcppd {

string HttpServer::handleRequest(const HttpRequest& request) {
  unique_ptr<string> uri = move(request.request_line->uri);
  return "";
}

HttpRequest HttpServer::parseRequest(const string& requestString) {
  yy_scan_string(requestString.c_str());
  yyparse();
  yylex_destroy();
  return move(*request);
}

string HttpServer::handleConnection() {
  std::string msg;
  sock_->Read(&msg);
  return handleRequest(parseRequest(msg));
}

void startDynamicContentServer() {
}

void HttpServer::serve() {
  syslog(LOG_INFO, "Server listening on %s", port.c_str());
  sock_.reset(new Socket(port));
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
