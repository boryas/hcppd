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

HttpResponse::HttpResponse(const std::string& message,
                           const std::string& content_type) :
    HttpResponse(200, "OK", message) {
  header["Content-Type"] = content_type;
  header["Content-Length"] = std::to_string(message.length());
}

std::string HttpResponse::format() const {
  std::string status = status_line.format();
  std::stringstream ss;
  ss << status << "\r\n";
  for (const auto& kv : header) {
    ss << kv.first << ": " << kv.second << "\r\n";
  }
  ss << "\r\n" << message;
  return ss.str();
}


} // namespace http
} // namespace lib
