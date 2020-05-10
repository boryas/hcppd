#pragma once

#include <string>

#include "lib/html/template.h"
#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/net/socket.h"
#include "lib/server/blocking.h"

namespace servers {
namespace blog {

class BlogServer : public ssfs::server::BlockingServer<BlogServer> {
public:
 BlogServer(const std::string& port);
 std::string handle(std::unique_ptr<ssfs::net::Socket> conn);
private:
 ssfs::http::HttpResponse handleRequest(
     const ssfs::http::HttpRequest& request) const;
 std::unique_ptr<ssfs::html::HtmlTemplate> template_;
};

} // namespace blog
} // namespace servers
