#pragma once

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

namespace lib {
namespace http {

class HttpStatusLine {
 public:
  HttpStatusLine() = default;
  HttpStatusLine(const std::string& protocol,
                 int status,
                 const std::string& reason);
  std::string protocol_version;
  int status_code;
  std::string reason_phrase;

  std::string format() const {
    std::stringstream ss;
    ss << protocol_version << " " << status_code << " "
       << reason_phrase << "\r\n";
    return ss.str();
  }
};

class HttpResponse {
 public:
  HttpResponse() = default;
  HttpResponse(int status,
               const std::string& reason,
               const std::string& message);
  HttpStatusLine status_line;
  std::string message;
  std::vector<std::string> header;

  std::string format() const {
    std::string status = status_line.format();
    std::stringstream ss;
    ss << status
       << "\r\n"
       << "<!DOCTYPE html>"
       << "<html><title>boris make page</title>"
       << "<body>" << message << "</body></html>";
    return ss.str();
  }
};

} // namespace http
} // namespace lib
