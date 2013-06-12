#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "server.h"
#include "http.tab.hpp"

#define SERV_PORT 8080

using namespace sock;
using namespace hcppd;
using namespace std;

extern int yyparse();
extern int yylex_destroy();
extern int yy_scan_string(const char *str);
extern HttpRequest *request;

HttpResponse HttpServer::handleRequest(const HttpRequest& request) {
  HttpResponse response;
  StatusLine status_line;
  status_line.protocol_version = "HTTP/1.1";

  unique_ptr<string> uri = move(request.request_line->uri);
  syslog(LOG_INFO, "Responding to request for: %s", uri->c_str());
  struct stat st;
  if (stat(uri->c_str(), &st) == -1) {
    if (errno == ENOENT) {
      status_line.status_code = 404;
      status_line.reason_phrase = "Not Found!";
    }
  }
  else {
    status_line.status_code = 200;
    status_line.reason_phrase = "OK";
    if (st.st_mode & S_IFDIR) {
      response.message = "dir";
    }
    else if (st.st_mode & S_IFREG) {
      response.message = "regular file";
    }
    else {
      response.message = "somethin' else";
    }
  }

  response.status_line = status_line;
  return response;
}

HttpRequest HttpServer::parseRequest(const string& requestString) {
  yy_scan_string(requestString.c_str());
  yyparse();
  yylex_destroy();
  return move(*request);
}

HttpResponse HttpServer::handleConnection() {
  std::string msg;
  sock_.Read(&msg);
  return handleRequest(parseRequest(msg));
}

void HttpServer::sendResponse(const HttpResponse& response) {
  sock_.Write(response.format());
}

void HttpServer::serve() {
  sock_.Bind(SERV_PORT);
  sock_.Listen();
  int x = 3;
  for ( ; ; ) {
    socklen_t clilen;
    sock_.Accept(&clilen);
    int connfd = sock_.getConnFd();
    int pid = fork();
    if (pid < 0) {
      // error
      syslog(LOG_WARNING, "failed to fork child to handle connection");
    } else if (pid == 0) {
      // child
      close(sock_.getListenFd());
      sendResponse(handleConnection());
      exit(0);
    }
    else {
      // parent
      close(sock_.getConnFd());
    }
  }
}
