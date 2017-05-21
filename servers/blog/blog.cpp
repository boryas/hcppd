#include "blog.h"

#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "lib/fs/fs.h"
#include "lib/http/parse/parser.h"
#include "lib/run/daemon.h"
#include "lib/run/options.h"

#include "resource.h"

namespace servers {
namespace blog {

auto constexpr TEMPLATE_FILE = "/home/bb/blog/templates/post.html";

BlogServer::BlogServer(const std::string& port)
 : ssfs::server::BlockingServer<BlogServer>(port) {
   auto t = std::make_unique<std::string>(
       ssfs::fs::readFile(TEMPLATE_FILE));
   template_.reset(new ssfs::html::HtmlTemplate(std::move(t)));
}

ssfs::http::HttpResponse BlogServer::handleRequest(
    const ssfs::http::HttpRequest& request) const {
  syslog(LOG_INFO, "Responding to request for: %s", request.uri().c_str());
  try {
    auto resource_type = resolveResourceType(request.uri());
    if (resource_type == ResourceType::BLOG_POST) {
      BlogPost post(request.uri());
      std::unordered_map<std::string, std::string> contents = {
        {"title", request.uri()},
        {"post", post.get()}
      };
      auto html = template_->populate(contents);
      return ssfs::http::HttpResponse(html, "text/html");
    } else {
      Css css(request.uri());
      return ssfs::http::HttpResponse(css.get(), "text/css");
    }
  } catch (ssfs::fs::PathNotFoundError& e) {
    ssfs::http::HttpResponse response(404, "Not Found!", "404 LOL");
    return response;
  } catch (std::exception& e) {
    ssfs::http::HttpResponse response(500, "Server Error!", "500 LOL");
    return response;
  }
}

std::string BlogServer::handle(std::unique_ptr<ssfs::net::Socket> conn) {
  syslog(LOG_INFO, "handle!");
  auto buf = std::make_unique<std::string>();
  buf->resize(4096);
  conn->readn(*buf, 4096);
  syslog(LOG_INFO, "handle buf %s", buf->c_str());
  ssfs::http::parse::HttpParser http_parser;
  while (http_parser.hungry()) {
    syslog(LOG_INFO, "hungry parser!");
    http_parser.consume(std::move(buf));
  }
  syslog(LOG_INFO, "full parser!");
  auto r = handleRequest(http_parser.request()).format();
  syslog(LOG_INFO, "Responding with: \n%s", r.c_str());
  conn->write_(r);
  return r;
}

} //namespace blog
} //namespace servers


int main(int argc, char **argv) {
  ssfs::run::daemonize();
  auto options = ssfs::run::get_options(argc, argv);
  ssfs::run::log_options(options);
  servers::blog::BlogServer server(options["port"]);
  server.serve();
}
