#pragma once

#include <string>
#include <syslog.h>
#include <memory>
#include <vector>

namespace ssfs {
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
  HttpMethod method;
  std::string uri;
  std::string protocol_version;
  std::string dumpMethod() {
    switch (method) {
      case HttpMethod::OPTIONS:
        return "OPTIONS";
      case HttpMethod::GET:
        return "GET";
      case HttpMethod::HEAD:
        return "HEAD";
      case HttpMethod::POST:
        return "POST";
      case HttpMethod::PUT:
        return "PUT";
      case HttpMethod::DELETE:
        return "DELETE";
      case HttpMethod::TRACE:
        return "TRACE";
      case HttpMethod::CONNECT:
        return "CONNECT";
      default:
        syslog(LOG_WARNING, "shouldn't have parsed...");
        return "error";
    }
  }
};

class HttpHeaderLine {
public:
  virtual ~HttpHeaderLine() {}
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
      case HttpGeneralHeaderField::CONNECTION:
        return "CONNECTION";
      case HttpGeneralHeaderField::DATE:
        return "DATE";
      case HttpGeneralHeaderField::PRAGMA:
        return "PRAGMA";
      case HttpGeneralHeaderField::TRAILER:
        return "TRAILER";
      case HttpGeneralHeaderField::TRANSFER_ENCODING:
        return "TRANSFER_ENCODING";
      case HttpGeneralHeaderField::UPGRADE:
        return "UPGRADE";
      case HttpGeneralHeaderField::VIA:
        return "VIA";
      case HttpGeneralHeaderField::WARNING:
        return "WARNING";
      default:
        syslog(LOG_WARNING, "shouldn't have parsed...");
        return "error";
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
      case HttpRequestHeaderField::ACCEPT_CHARSET:
        return "ACCEPT_CHARSET";
      case HttpRequestHeaderField::ACCEPT_ENCODING:
        return "ACCEPT_ENCODING";
      case HttpRequestHeaderField::ACCEPT_LANGUAGE:
        return "ACCEPT_LANGUAGE";
      case HttpRequestHeaderField::AUTHORIZATION:
        return "AUTHORIZATION";
      case HttpRequestHeaderField::EXPECT:
        return "EXPECT";
      case HttpRequestHeaderField::FROM:
        return "FROM";
      case HttpRequestHeaderField::HOST:
        return "HOST";
      case HttpRequestHeaderField::IF_MATCH:
        return "IF_MATCH";
      case HttpRequestHeaderField::IF_MODIFIED_SINCE:
        return "IF_MODIFIED_SINCE";
      case HttpRequestHeaderField::IF_NONE_MATCH:
        return "IF_NONE_MATCH";
      case HttpRequestHeaderField::IF_RANGE:
        return "IF_RANGE";
      case HttpRequestHeaderField::IF_UNMODIFIED_SINCE:
        return "IF_UNMODIFIED_SINCE";
      case HttpRequestHeaderField::MAX_FORWARDS:
        return "MAX_FORWARDS";
      case HttpRequestHeaderField::PROXY_AUTHORIZATION:
        return "PROXY_AUTHORIZATION";
      case HttpRequestHeaderField::RANGE:
        return "RANGE";
      case HttpRequestHeaderField::REFERER:
        return "REFERER";
      case HttpRequestHeaderField::TE:
        return "TE";
      case HttpRequestHeaderField::USER_AGENT:
        return "USER_AGENT";
      default:
        syslog(LOG_WARNING, "shouldn't have parsed...");
        return "error";
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
      case HttpEntityHeaderField::CONTENT_ENCODING:
        return "CONTENT_ENCODING";
      case HttpEntityHeaderField::CONTENT_LANGUAGE:
        return "CONTENT_LANGUAGE";
      case HttpEntityHeaderField::CONTENT_LENGTH:
        return "CONTENT_LENGTH";
      case HttpEntityHeaderField::CONTENT_LOCATION:
        return "CONTENT_LOCATION";
      case HttpEntityHeaderField::CONTENT_MD5:
        return "CONTENT_MD5";
      case HttpEntityHeaderField::CONTENT_RANGE:
        return "CONTENT_RANGE";
      case HttpEntityHeaderField::CONTENT_TYPE:
        return "CONTENT_TYPE";
      case HttpEntityHeaderField::EXPIRES:
        return "EXPIRES";
      case HttpEntityHeaderField::LAST_MODIFIED:
        return "LAST_MODIFIED";
      default:
        syslog(LOG_WARNING, "shouldn't have parsed...");
        return "error";
    }
  }
};

class HttpRequest {
public:
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
} // namespace ssfs
