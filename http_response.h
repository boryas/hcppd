#ifndef _HTTP_RESPONSE_DEFS_H
#define _HTTP_RESPONSE_DEFS_H

#include<iostream>
#include<sstream>
#include<string>
#include<vector>
namespace hcppd {

class StatusLine {
public:
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
  StatusLine status_line;
  //TODO: massage imports to share general header fields and
  //exhaust/enumerate the actual fields
  std::vector<std::string> header;
  std::string message;
  std::string format() const {
    std::string status = status_line.format();
    std::stringstream ss;
    ss << status
       << "\r\n"
       << "<html><title>boris make page</title>"
       << "<body>" << message << "</body></html>";
    return ss.str();
  }
};
}
#endif //_HTTP_RESPONSE_DEFS_H
