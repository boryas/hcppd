#include "request.h"

#include <syslog.h>

#include <sstream>

namespace lib {
namespace http {

HttpRequestLine::HttpRequestLine(const std::string& raw_request_line) {
  std::stringstream ss(raw_request_line);
  std::string token;
  std::getline(ss, token, ' ');
  if (token == "GET") {
    method = HttpMethod::GET;
  } else if (token == "POST") {
    method = HttpMethod::POST;
  }
  std::getline(ss, token, ' ');
  uri = token;
  std::getline(ss, token, ' ');
  protocol_version = token;
}

} // namespace http
} // namespace lib
