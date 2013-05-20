#include "server.h"

using namespace bostack::sock;
using namespace bostack::httpd;
using namespace std;

HttpResponse HttpServer::handleRequest(const HttpRequest& request) {
  return "dummy response!";
}

HttpRequest HttpServer::parseRequest(const string& requestString) {
  cout << requestString;
  return requestString;
}

HttpResponse HttpServer::handleConnection() {
  std::string msg;
  sock_.Read(&msg);
  return handleRequest(parseRequest(msg));
}

void HttpServer::sendResponse(const HttpResponse& response) {
  sock_.Write(response);
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
