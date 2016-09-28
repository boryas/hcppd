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

HttpRequest::HttpRequest(const std::string& raw_request) {
  std::stringstream ss(raw_request);
  std::string raw_rl;
  std::getline(ss, raw_rl, '\n');
  syslog(LOG_INFO, raw_rl.c_str());
  HttpRequestLine rl(raw_rl);
  request_line = rl;
}

} // namespace http
} // namespace lib
