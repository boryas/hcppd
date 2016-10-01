#pragma once

#include <string>

#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"

namespace servers {

class FsServer : public lib::server::BlockingServer<FsServer> {
public:
 FsServer(const std::string& port)
   : lib::server::BlockingServer<FsServer>(port) {}
 std::string handle(const std::string& message);
private:
 lib::http::HttpResponse handleRequest(const lib::http::HttpRequest& request);
};

} // namespace servers
