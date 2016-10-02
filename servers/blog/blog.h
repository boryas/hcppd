#pragma once

#include <string>

#include "lib/html.h"
#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"

namespace servers {
namespace blog {

class BlogServer : public lib::server::BlockingServer<BlogServer> {
public:
 BlogServer(const std::string& port)
   : lib::server::BlockingServer<BlogServer>(port),
     template_(std::make_unique<std::string>(
           "<!DOCTYPE html>"
           "<link rel=\"stylesheet\" type=\"text/css\" href=\"foo.css\">"
           "<html>"
           "<title>{{title}}</title>"
           "<body><p>{{body}}</p></body>"
           "</html>"
           )) {}
 std::string handle(const std::string& message);
private:
 lib::http::HttpResponse handleRequest(
     const lib::http::HttpRequest& request) const;
 lib::html::HtmlTemplate template_;
};

} // namespace blog
} // namespace servers
