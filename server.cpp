#include "server.h"
#include "http.tab.hpp"

using namespace sock;
using namespace hcppd;
using namespace std;

extern int yyparse();
extern int yy_scan_string(const char *str);
extern HttpRequest *request;

HttpResponse HttpServer::handleRequest(const HttpRequest& request) {
  HttpResponse response;
  return response;
}

HttpRequest HttpServer::parseRequest(const string& requestString) {
  yy_scan_string(requestString.c_str());
  yyparse();
  unique_ptr<HttpRequestLine> rl;
  rl = move(request->request_line);
  cout << "---------\nREQUEST:\n";
  cout << "protocol version: " << *rl->protocol_version << endl;
  cout << "method: " << rl->dumpMethod() << endl;
  cout << "uri: " << *rl->uri << endl;

  cout << "---------\nHEADER:\n";
  unique_ptr<vector<unique_ptr<HttpHeaderLine>>> h;
  h = move(request->header);
  for (int i=0; i<h->size(); ++i) {
    unique_ptr<HttpHeaderLine> hl;
    hl = move((*h)[i]);
    cout << hl->dumpField() << ": " << *hl->value << endl;
  }
  cout << "---------\n";

  return move(*request);
}

HttpResponse HttpServer::handleConnection() {
  std::string msg;
  sock_.Read(&msg);
  return handleRequest(parseRequest(msg));
}

void HttpServer::sendResponse(const HttpResponse& response) {
  sock_.Write("dummy response");
}

void HttpServer::serve() {
  sock_.Bind();
  sock_.Listen();
  int x = 3;
  for ( ; ; ) {
    socklen_t clilen;
    sock_.Accept(&clilen);
    int connfd = sock_.getConnFd();
    int pid = fork();
    if (pid < 0) {
      // error
      std::cout << "failed to fork child to handle connection\n";
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
