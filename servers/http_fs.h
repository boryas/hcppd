#pragma once

#include <string>

#include "../lib/http/request.h"
#include "../lib/http/response.h"
#include "../lib/server.h"

namespace servers {

class HttpFsServer : public lib::server::BlockingServer<HttpFsServer> {
public:
 HttpFsServer(const std::string& port)
   : lib::server::BlockingServer<HttpFsServer>(port) {}
 std::string handle(const std::string& message);
private:
 lib::http::HttpResponse handleRequest(const lib::http::HttpRequest& request);
};

} // hcppd
