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
  std::string uri;
  HttpMethod method;
  std::string protocol_version;
};

class HttpHeaderLine {
public:
  virtual ~HttpHeaderLine() {}
  virtual void print_field() = 0;
  std::string value;
  HEADER_TYPE type;
};

class HttpGeneralHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpGeneralHeaderLine() {}
  HttpGeneralHeaderField field;
  virtual void print_field() {
    switch (field) {
      case CACHE_CONTROL: std::cout << "CACHE_CONTROL"; break;
      case CONNECTION: std::cout << "CONNECTION"; break;
      case DATE: std::cout << "DATE"; break;
      case PRAGMA: std::cout << "PRAGMA"; break;
      case TRAILER: std::cout << "TRAILER"; break;
      case TRANSFER_ENCODING: std::cout << "TRANSFER_ENCODING"; break;
      case UPGRADE: std::cout << "UPGRADE"; break;
      case VIA: std::cout << "VIA"; break;
      case WARNING: std::cout << "WARNING"; break;
      default: std::cout << "shouldn't have parsed...";
    }
  }
};

class HttpRequestHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpRequestHeaderLine() {}
  HttpRequestHeaderField field;
  virtual void print_field() {
    switch (field) {
      case ACCEPT: std::cout << "ACCEPT"; break;
      case ACCEPT_CHARSET: std::cout << "ACCEPT_CHARSET"; break;
      case ACCEPT_ENCODING: std::cout << "ACCEPT_ENCODING"; break;
      case ACCEPT_LANGUAGE: std::cout << "ACCEPT_LANGUAGE"; break;
      case AUTHORIZATION: std::cout << "AUTHORIZATION"; break;
      case EXPECT: std::cout << "EXPECT"; break;
      case FROM: std::cout << "FROM"; break;
      case HOST: std::cout << "HOST"; break;
      case IF_MATCH: std::cout << "IF_MATCH"; break;
      case USER_AGENT: std::cout << "USER_AGENT"; break;
      default: std::cout << "shouldn't have parsed...";
    }
  }
};

class HttpEntityHeaderLine : public HttpHeaderLine {
public:
  virtual ~HttpEntityHeaderLine() {}
  HttpEntityHeaderField field;
  virtual void print_field() {
    switch (field) {
      case ALLOW: std::cout << "ALLOW"; break;
      case CONTENT_ENCODING: std::cout << "CONTENT_ENCODING"; break;
      case CONTENT_LANGUAGE: std::cout << "CONTENT_LANGUAGE"; break;
      case CONTENT_LENGTH: std::cout << "CONTENT_LENGTH"; break;
      case CONTENT_LOCATION: std::cout << "CONTENT_LOCATION"; break;
      case CONTENT_MD5: std::cout << "CONTENT_MD5"; break;
      case CONTENT_RANGE: std::cout << "CONTENT_RANGE"; break;
      case CONTENT_TYPE: std::cout << "CONTENT_TYPE"; break;
      case EXPIRES: std::cout << "EXPIRES"; break;
      case LAST_MODIFIED: std::cout << "LAST_MODIFIED"; break;
      default: std::cout << "shouldn't have parsed...";
    }
  }
};

class HttpRequest {
public:
  virtual ~HttpRequest() {}
  HttpRequestLine request_line;
  std::vector<HttpHeaderLine *> header;
};

#endif //_HTTP_DEFS_H
