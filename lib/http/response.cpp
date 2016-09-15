#include "response.h"

namespace lib {
namespace http {
  
auto constexpr PROTOCOL_VERSION = "HTTP/1.1";

  HttpStatusLine::HttpStatusLine(const std::string& protocol,
                                 int status, 
                                 const std::string& reason) :
    protocol_version(protocol),
    status_code(status),
    reason_phrase(reason) {}

  HttpResponse::HttpResponse(int status,
                             const std::string& reason,
                             const std::string& message) :
    status_line(PROTOCOL_VERSION, status, reason),
    message(message) {}

} // namespace http
} // namespace lib
