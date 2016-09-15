#pragma once

#include <string>

#include "lib/http/request.h"
#include "lib/socket.h"

namespace hcppd {

class HttpServer {
public:
  std::string port = "80";
  std::string dynamic_content_server;
  void serve();
private:
  std::string handleRequest(const lib::http::HttpRequest& request);
  std::string handleConnection();
  std::unique_ptr<lib::sock::Socket> sock_;
};
} // server
