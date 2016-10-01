#include "fs.h"

#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#include "lib/daemon.h"
#include "lib/fs.h"
#include "lib/options.h"

namespace servers {

lib::http::HttpResponse FsServer::handleRequest(
    const lib::http::HttpRequest& request) {
  syslog(LOG_INFO, "Responding to request for: %s", request.uri().c_str());
  std::stringstream ss;
  try {
    lib::fs::Stat stat(request.uri());
    if (stat.isDir()) {
      lib::fs::Directory d(request.uri());
      auto contents = d.contents();
      for (const auto& c : contents) {
        ss << "<a href=\"" << request.uri() << "/" << c << "\">" <<
          request.uri() << "/" << c << "</a></br>\n";
      }
    } else if (stat.isRegularFile()) {
      lib::fs::File f(request.uri());
      auto lines = f.readLines();
      for (const auto& l : lines) {
        ss << l << "\n";
      }
    } else {
      throw std::runtime_error("bad file type, not a plain file or dir");
    }
  } catch (lib::fs::PathNotFoundError& e) {
    lib::http::HttpResponse response(404, "Not Found!", "404 LOL");
    return response;
  } catch (lib::fs::FsError& e) {
    lib::http::HttpResponse response(500, "Error with file!", "500 LOL");
    return response;
  }
  lib::http::HttpResponse response(200, "OK", ss.str());
  return response;
}

std::string FsServer::handle(const std::string& msg) {
  lib::http::HttpRequest req(msg);
  return handleRequest(req).format();
}

} //namespace servers

int main(int argc, char **argv) {
  lib::daemon::daemonize();
  auto options = lib::options::get_options(argc, argv);
  lib::options::log_options(options);
  servers::FsServer server(options["port"]);
  server.serve();
}
