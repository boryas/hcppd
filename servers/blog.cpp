#include "blog.h"

#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "lib/daemon.h"
#include "lib/fs.h"
#include "lib/options.h"

namespace servers {

lib::http::HttpResponse BlogServer::handleRequest(
    const lib::http::HttpRequest& request) const {
  syslog(LOG_INFO, "Responding to request for: %s", request.uri().c_str());
  if (request.uri().find(".css") != std::string::npos) {
    auto path = root_dir_ + "/css" + request.uri();
    syslog(LOG_INFO, "Finding css at %s", path.c_str());
    lib::fs::File f(path);
    auto css = f.read();
    return lib::http::HttpResponse(css, "text/css");
  }
  auto path = resolveUri(request.uri());
  syslog(LOG_INFO, "looking up post at: %s", path.c_str());
  std::stringstream ss;
  try {
    lib::fs::Stat stat(path);
    if (stat.isRegularFile()) {
      ss << "<p>";
      lib::fs::File f(path);
      ss << f.read();
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
  lib::http::HttpResponse response(
      template_.populate({{"title", request.uri()}, {"body", ss.str()}}),
      "text/html");
  return response;
}

std::string BlogServer::resolveUri(const std::string& uri) const {
  std::stringstream ss;
  ss << root_dir_ << uri;
  return ss.str();
}

std::string BlogServer::handle(const std::string& msg) {
  lib::http::HttpRequest req(msg);
  auto r = handleRequest(req).format();
  syslog(LOG_INFO, "Responding with: \n%s", r.c_str());
  return r;
}

} //namespace servers


int main(int argc, char **argv) {
  lib::daemon::daemonize();
  auto options = lib::options::get_options(argc, argv);
  lib::options::log_options(options);
  servers::BlogServer server(options["port"], "/home/bb/blog");
  server.serve();
}
