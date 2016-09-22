#pragma once

#include <string>

#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"
#include "lib/socket.h"

namespace hcppd {

class HttpFsServer : public lib::server::BlockingServer<HttpFsServer> {
public:
 std::string handle(const std::string& message);
private:
 lib::http::HttpResponse handleRequest(const lib::http::HttpRequest& request);
};

} // hcppd
