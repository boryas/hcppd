#pragma once

#include <string>
#include <syslog.h>
#include <memory>
#include <vector>

namespace lib {
namespace http {

enum class HttpMethod {
  OPTIONS,
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  TRACE,
  CONNECT,
};

enum class HttpGeneralHeaderField {
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

enum class HttpRequestHeaderField {
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

enum class HttpEntityHeaderField {
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

enum class HeaderType {
  GENERAL,
  REQUEST,
  ENTITY
};

class HttpRequestLine {
public:
  HttpRequestLine(const std::string& raw_request_line);
  HttpRequestLine() = default;
  virtual ~HttpRequestLine() {}
  std::string uri;
  HttpMethod method;
  std::string protocol_version;
  std::string dumpMethod() {
    switch (method) {
      case HttpMethod::OPTIONS:
        return "OPTIONS";
        break;
      case HttpMethod::GET:
        return "GET";
        break;
      case HttpMethod::HEAD:
        return "HEAD";
        break;
      case HttpMethod::POST:
        return "POST";
        break;
      case HttpMethod::PUT:
        return "PUT";
        break;
      case HttpMethod::DELETE:
        return "DELETE";
        break;
      case HttpMethod::TRACE:
        return "TRACE";
        break;
      case HttpMethod::CONNECT:
        return "CONNECT";
        break;
      default: return "shouldn't have parsed"; return "error";
    }
  }
};

class HttpHeaderLine {
public:
  virtual ~HttpHeaderLine() {}
  virtual std::string dumpField() = 0;
  std::string value;
  HeaderType type;
};

class HttpGeneralHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpGeneralHeaderLine() {}
  HttpGeneralHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case HttpGeneralHeaderField::CACHE_CONTROL:
        return "CACHE_CONTROL";
        break;
      case HttpGeneralHeaderField::CONNECTION:
        return "CONNECTION";
        break;
      case HttpGeneralHeaderField::DATE:
        return "DATE";
        break;
      case HttpGeneralHeaderField::PRAGMA:
        return "PRAGMA";
        break;
      case HttpGeneralHeaderField::TRAILER:
        return "TRAILER";
        break;
      case HttpGeneralHeaderField::TRANSFER_ENCODING:
        return "TRANSFER_ENCODING";
        break;
      case HttpGeneralHeaderField::UPGRADE:
        return "UPGRADE";
        break;
      case HttpGeneralHeaderField::VIA:
        return "VIA";
        break;
      case HttpGeneralHeaderField::WARNING:
        return "WARNING";
        break;
      default: syslog(LOG_WARNING, "shouldn't have parsed..."); return "error";
    }
  }
};

class HttpRequestHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpRequestHeaderLine() {}
  HttpRequestHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case HttpRequestHeaderField::ACCEPT:
        return "ACCEPT";
        break;
      case HttpRequestHeaderField::ACCEPT_CHARSET:
        return "ACCEPT_CHARSET";
        break;
      case HttpRequestHeaderField::ACCEPT_ENCODING:
        return "ACCEPT_ENCODING";
        break;
      case HttpRequestHeaderField::ACCEPT_LANGUAGE:
        return "ACCEPT_LANGUAGE";
        break;
      case HttpRequestHeaderField::AUTHORIZATION:
        return "AUTHORIZATION";
        break;
      case HttpRequestHeaderField::EXPECT:
        return "EXPECT";
        break;
      case HttpRequestHeaderField::FROM:
        return "FROM";
        break;
      case HttpRequestHeaderField::HOST:
        return "HOST";
        break;
      case HttpRequestHeaderField::IF_MATCH:
        return "IF_MATCH";
        break;
      case HttpRequestHeaderField::IF_MODIFIED_SINCE:
        return "IF_MODIFIED_SINCE";
        break;
      case HttpRequestHeaderField::IF_NONE_MATCH:
        return "IF_NONE_MATCH";
        break;
      case HttpRequestHeaderField::IF_RANGE:
        return "IF_RANGE";
        break;
      case HttpRequestHeaderField::IF_UNMODIFIED_SINCE:
        return "IF_UNMODIFIED_SINCE";
        break;
      case HttpRequestHeaderField::MAX_FORWARDS:
        return "MAX_FORWARDS";
        break;
      case HttpRequestHeaderField::PROXY_AUTHORIZATION:
        return "PROXY_AUTHORIZATION";
        break;
      case HttpRequestHeaderField::RANGE:
        return "RANGE";
        break;
      case HttpRequestHeaderField::REFERER:
        return "REFERER";
        break;
      case HttpRequestHeaderField::TE:
        return "TE";
        break;
      case HttpRequestHeaderField::USER_AGENT:
        return "USER_AGENT";
        break;
      default: syslog(LOG_WARNING, "shouldn't have parsed..."); return "error";
    }
  }
};

class HttpEntityHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpEntityHeaderLine() {}
  HttpEntityHeaderField field;
  virtual std::string dumpField() {
    switch (field) {
      case HttpEntityHeaderField::ALLOW:
        return "ALLOW";
        break;
      case HttpEntityHeaderField::CONTENT_ENCODING:
        return "CONTENT_ENCODING";
        break;
      case HttpEntityHeaderField::CONTENT_LANGUAGE:
        return "CONTENT_LANGUAGE";
        break;
      case HttpEntityHeaderField::CONTENT_LENGTH:
        return "CONTENT_LENGTH";
        break;
      case HttpEntityHeaderField::CONTENT_LOCATION:
        return "CONTENT_LOCATION";
        break;
      case HttpEntityHeaderField::CONTENT_MD5:
        return "CONTENT_MD5";
        break;
      case HttpEntityHeaderField::CONTENT_RANGE:
        return "CONTENT_RANGE";
        break;
      case HttpEntityHeaderField::CONTENT_TYPE:
        return "CONTENT_TYPE";
        break;
      case HttpEntityHeaderField::EXPIRES:
        return "EXPIRES";
        break;
      case HttpEntityHeaderField::LAST_MODIFIED:
        return "LAST_MODIFIED";
        break;
      default: syslog(LOG_WARNING, "shouldn't have parsed...");return "error";
    }
  }
};

class HttpRequest {
public:
  HttpRequest(const std::string& raw_request);
  virtual ~HttpRequest() {}
  HttpRequestLine request_line;
  std::vector<HttpHeaderLine> header;
  HttpMethod method() const {
    return request_line.method;
  };
  std::string uri() const {
    return request_line.uri;
  };
};

} // namespace http
} // namespace lib
