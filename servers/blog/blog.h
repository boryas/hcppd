#pragma once

#include <string>

#include "lib/html.h"
#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"
#include "lib/socket.h"

namespace servers {
namespace blog {

class BlogServer : public lib::server::BlockingServer<BlogServer> {
public:
 BlogServer(const std::string& port);
 std::string handle(std::unique_ptr<lib::sock::Socket> buf);
private:
 lib::http::HttpResponse handleRequest(
     const lib::http::HttpRequest& request) const;
 std::unique_ptr<lib::html::HtmlTemplate> template_;
};

} // namespace blog
} // namespace servers
