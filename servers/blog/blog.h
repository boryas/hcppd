#pragma once

#include <string>

#include "lib/html.h"
#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"
#include "lib/socket.h"

namespace servers {
namespace blog {

class BlogServer : public ssfs::server::BlockingServer<BlogServer> {
public:
 BlogServer(const std::string& port);
 std::string handle(std::unique_ptr<ssfs::sock::Socket> conn);
private:
 ssfs::http::HttpResponse handleRequest(
     const ssfs::http::HttpRequest& request) const;
 std::unique_ptr<ssfs::html::HtmlTemplate> template_;
};

} // namespace blog
} // namespace servers
