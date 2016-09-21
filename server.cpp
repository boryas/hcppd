#include "server.h"

#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "lib/fs.h"
#include "lib/http/response.h"

namespace hcppd {

std::string HttpServer::handleRequest(const lib::http::HttpRequest& request) {
  syslog(LOG_INFO, "Responding to request for: %s", request.uri().c_str());
  std::stringstream ss;
  try {
    lib::fs::Stat stat(request.uri());
    if (stat.isDir()) {
      lib::fs::Directory d(request.uri());
      d.read();
      for (const auto& c : d.contents) {
        ss << "<a href=\"" << request.uri() << "/" << c << "\">" <<
          request.uri() << "/" << c << "</a><br>" << std::endl;
      }
    } else if (stat.isRegularFile()) {
      lib::fs::File f(request.uri());
      f.read();
      for (const auto& l : f.lines) {
        ss << l << "<br>" << std::endl;
      }
    } else {
      throw std::runtime_error("bad file type, not a plain file or dir");
    }
  } catch (lib::fs::PathNotFoundError& e) {
    lib::http::HttpResponse response(404, "Not Found!", "404 LOL");
    return response.format();
  } catch (lib::fs::FsError& e) {
    lib::http::HttpResponse response(500, "Error stat-ing file!", "500 LOL");
    return response.format();
  }
  lib::http::HttpResponse response(200, "OK", ss.str());
  return response.format();
}

std::string HttpServer::handleConnection() {
  std::string msg;
  sock_->Read(&msg);
  lib::http::HttpRequest req(msg);
  syslog(LOG_INFO, req.request_line.uri.c_str());
  auto response = handleRequest(req);
  syslog(LOG_INFO, "response: %s", response.c_str());
  return response;
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
