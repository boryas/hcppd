#ifndef _HTTP_REQUEST_DEFS_H
#define _HTTP_REQUEST_DEFS_H

#include <string>
#include <syslog.h>
#include <memory>
#include <vector>

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

enum HttpGeneralHeaderField {
  CACHE_CONTROL,
  CONNECTION,
  DATE,
  PRAGMA,
  TRAILER,
  TRANSFER_ENCODING,
  UPGRADE,
  VIA,
  WARNING,
};

enum HttpRequestHeaderField {
  ACCEPT,
  ACCEPT_CHARSET,
  ACCEPT_ENCODING,
  ACCEPT_LANGUAGE,
  AUTHORIZATION,
  EXPECT,
  FROM,
  HOST,
  IF_MATCH,
  IF_MODIFIED_SINCE,
  IF_NONE_MATCH,
  IF_RANGE,
  IF_UNMODIFIED_SINCE,
  MAX_FORWARDS,
  PROXY_AUTHORIZATION,
  RANGE,
  REFERER,
  TE,
  USER_AGENT,
};

enum HttpEntityHeaderField {
  ALLOW,
  CONTENT_ENCODING,
  CONTENT_LANGUAGE,
  CONTENT_LENGTH,
  CONTENT_LOCATION,
  CONTENT_MD5,
  CONTENT_RANGE,
  CONTENT_TYPE,
  EXPIRES,
  LAST_MODIFIED,
};

enum HEADER_TYPE {
  GENERAL,
  REQUEST,
  ENTITY
};

class HttpRequestLine {
public:
  virtual ~HttpRequestLine() {}
  std::unique_ptr<std::string> uri;
  HttpMethod method;
  std::unique_ptr<std::string> protocol_version;
  std::string dumpMethod() {
    switch (method) {
      case OPTIONS: return "OPTIONS";
      case GET: return "GET";
      case HEAD: return "HEAD";
      case POST: return "POST";
      case PUT: return "PUT";
      case DELETE: return "DELETE";
      case TRACE: return "TRACE";
      case CONNECT: return "CONNECT";
      default: return "shouldn't have parsed";
    }
  }
};

class HttpHeaderLine {
public:
  virtual ~HttpHeaderLine() {}
  virtual std::string dumpField() = 0;
  std::unique_ptr<std::string> value;
  HEADER_TYPE type;
};

class HttpGeneralHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpGeneralHeaderLine() {}
  HttpGeneralHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case CACHE_CONTROL: return "CACHE_CONTROL";
      case CONNECTION: return "CONNECTION";
      case DATE: return "DATE";
      case PRAGMA: return "PRAGMA";
      case TRAILER: return "TRAILER";
      case TRANSFER_ENCODING: return "TRANSFER_ENCODING";
      case UPGRADE: return "UPGRADE";
      case VIA: return "VIA";
      case WARNING: return "WARNING";
      default: syslog(LOG_WARNING, "shouldn't have parsed...");
    }
  }
};

class HttpRequestHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpRequestHeaderLine() {}
  HttpRequestHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case ACCEPT: return "ACCEPT";
      case ACCEPT_CHARSET: return "ACCEPT_CHARSET";
      case ACCEPT_ENCODING: return "ACCEPT_ENCODING";
      case ACCEPT_LANGUAGE: return "ACCEPT_LANGUAGE";
      case AUTHORIZATION: return "AUTHORIZATION";
      case EXPECT: return "EXPECT";
      case FROM: return "FROM";
      case HOST: return "HOST";
      case IF_MATCH: return "IF_MATCH";
      case IF_MODIFIED_SINCE: return "IF_MODIFIED_SINCE";
      case IF_NONE_MATCH: return "IF_NONE_MATCH";
      case IF_RANGE: return "IF_RANGE";
      case IF_UNMODIFIED_SINCE: return "IF_UNMODIFIED_SINCE";
      case MAX_FORWARDS: return "MAX_FORWARDS";
      case PROXY_AUTHORIZATION: return "PROXY_AUTHORIZATION";
      case RANGE: return "RANGE";
      case REFERER: return "REFERER";
      case TE: return "TE";
      case USER_AGENT: return "USER_AGENT";
      default: syslog(LOG_WARNING, "shouldn't have parsed...");
    }
  }
};

class HttpEntityHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpEntityHeaderLine() {}
  HttpEntityHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case ALLOW: return "ALLOW";
      case CONTENT_ENCODING: return "CONTENT_ENCODING";
      case CONTENT_LANGUAGE: return "CONTENT_LANGUAGE";
      case CONTENT_LENGTH: return "CONTENT_LENGTH";
      case CONTENT_LOCATION: return "CONTENT_LOCATION";
      case CONTENT_MD5: return "CONTENT_MD5";
      case CONTENT_RANGE: return "CONTENT_RANGE";
      case CONTENT_TYPE: return "CONTENT_TYPE";
      case EXPIRES: return "EXPIRES";
      case LAST_MODIFIED: return "LAST_MODIFIED";
      default: syslog(LOG_WARNING, "shouldn't have parsed...");
    }
  }
};

class HttpRequest {
public:
  HttpRequest() {}
  HttpRequest(HttpRequest&& request) :
    request_line(std::move(request.request_line)),
    header(std::move(request.header)) {}
  HttpRequest &operator=(HttpRequest&& request) {
    request_line = std::move(request.request_line);
    header = std::move(request.header);
    return *this;
  }
  virtual ~HttpRequest() {}
  std::unique_ptr<HttpRequestLine> request_line;
  std::unique_ptr<std::vector<std::unique_ptr<HttpHeaderLine>>> header;
private:
  HttpRequest(const HttpRequest& request);
  HttpRequest &operator=(const HttpRequest& request);
};

#endif //_HTTP_REQUEST_DEFS_H
