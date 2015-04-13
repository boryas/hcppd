#include "http_request.h"
#include "http_response.h"
#include "socket.h"

namespace hcppd {

const int SERV_PORT = 80;

class HttpServer {
public:
  int port = SERV_PORT;
  std::string dynamic_content_server;
  void serve();
private:
  HttpResponse respond(int status,
                       const std::string& reason,
                       const std::string& message);
  HttpResponse handleRequest(const HttpRequest& request);
  HttpRequest parseRequest(const std::string& requestString);
  HttpResponse handleConnection();
  void sendResponse(const HttpResponse& response);
  std::unique_ptr<sock::Socket> sock_;
};
} // server
