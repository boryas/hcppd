#pragma once

#include <string>

#include "lib/html.h"
#include "lib/http/request.h"
#include "lib/http/response.h"
#include "lib/server.h"

namespace servers {

class BlogServer : public lib::server::BlockingServer<BlogServer> {
public:
 BlogServer(const std::string& port, const std::string& root_dir)
   : lib::server::BlockingServer<BlogServer>(port),
     root_dir_(root_dir),
     template_(std::make_unique<std::string>(
           "<!DOCTYPE html>"
           "<html>"
           "<title>{{title}}</title>"
           "<body>{{body}}</body>"
           "</html>"
           )) {}
 std::string handle(const std::string& message);
private:
 lib::http::HttpResponse handleRequest(
     const lib::http::HttpRequest& request) const;
 std::string resolveUri(const std::string& uri) const;
 std::string root_dir_;
 lib::html::HtmlTemplate template_;
};

} // namespace servers
