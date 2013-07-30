#include "http_request.h"
#include "http_response.h"
#include "socket.h"

namespace hcppd {
class HttpServer {
public:
  void serve();
private:
  HttpResponse respond(int status,
                       const std::string& reason,
                       const std::string& message);
  HttpResponse handleRequest(const HttpRequest& request);
  HttpRequest parseRequest(const std::string& requestString);
  HttpResponse handleConnection();
  void sendResponse(const HttpResponse& response);
  sock::Socket sock_;
};
} // server
