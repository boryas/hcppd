#ifndef _HTTP_DEFS_H
#define _HTTP_DEFS_H

#include <vector>
#include <string>

enum HttpMethod {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  TRACE,
  CONNECT,
};

class HttpRequestLine {
public:
  virtual ~HttpRequestLine() {}
  std::string uri;
  HttpMethod method;
  std::string protocol_version;
};

class HttpHeaderLine {
public:
  virtual ~HttpHeaderLine() {}
  std::string name;
  std::string value;
};

class HttpRequest {
public:
  virtual ~HttpRequest() {
    ~request_line;
  }
  HttpRequestLine request_line;
  std::vector<HttpHeaderLine> header;
};

#endif //_HTTP_DEFS_H
