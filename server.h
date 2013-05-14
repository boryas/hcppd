#include "socket.h"

namespace bostack {
namespace httpd {
typedef std::string HttpResponse;
typedef std::string HttpRequest;
class HttpServer {
public:
  void serve();
private:
  HttpResponse handleRequest(const HttpRequest& request);
  HttpRequest parseRequest(const std::string& requestString);
  HttpResponse handleConnection();
  void sendResponse(const HttpResponse& response);
  bostack::sock::Socket sock_;
};
} // server
} // bostack
