#include "blog.h"

#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "lib/fs.h"

namespace servers {

lib::http::HttpResponse BlogServer::handleRequest(
    const lib::http::HttpRequest& request) const {
  syslog(LOG_INFO, "Responding to request for: %s", request.uri().c_str());
  auto path = resolveUri(request.uri());
  syslog(LOG_INFO, "looking up post at: %s", path.c_str());
  std::stringstream ss;
  try {
    lib::fs::Stat stat(path);
    if (stat.isRegularFile()) {
      ss << "<p>";
      lib::fs::File f(path);
      f.read();
      for (const auto& l : f.lines) {
        ss << l;
      }
      ss << "</p>";
    } else {
      lib::http::HttpResponse response(404, "Not Found!", "404 LOL");
      return response;
    }
  } catch (lib::fs::PathNotFoundError& e) {
    lib::http::HttpResponse response(404, "Not Found!", "404 LOL");
    return response;
  } catch (lib::fs::FsError& e) {
    lib::http::HttpResponse response(500, "Error with file!", "500 LOL");
    return response;
  }
  lib::http::HttpResponse response(200, "OK",
      template_.populate({{"title", request.uri()}, {"body", ss.str()}}));
  return response;
}

std::string BlogServer::resolveUri(const std::string& uri) const {
  std::stringstream ss;
  ss << root_dir_ << uri;
  return ss.str();
}

std::string BlogServer::handle(const std::string& msg) {
  lib::http::HttpRequest req(msg);
  return handleRequest(req).format();
}

} //namespace servers
