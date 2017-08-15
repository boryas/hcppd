#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "lib/http/request.h"
#include "lib/string/view.h"

namespace ssfs {
namespace http {
namespace parse {

class ParserError : public std::runtime_error {
 public:
  ParserError(const std::string& reason)
    : std::runtime_error("http request ParserError: " + reason) {}
};

/*
 * Parser Design (not yet implemented)
 * The parser is actually a hierarchy of parsers. Each parser has an
 * ordered list of sub-parsers and it gives data to each to consume until
 * the sub-parser is satisfied. A Parser's consume method will return how
 * much data it consumed so that it's parent can give the rest of the data
 * to the next sub-parser.
 * The data will be passed around in "string views" so we never have to copy to
 * slice/index into strings when chopping them up and passing them around
 */

// OPEN Qs:
// 1: How to pass output request object around?
// option: take a ref as an arg of consume
// option: take a ref as an arg of ctor (feasible?)
// option: get ownership as an arg of ctor, have method to relinquish
//
//
// TODO: make a string view class to avoid all the gratuitous copying

class Parser {
 public:
  virtual ~Parser() {}
  virtual size_t consume(string::StringView chunk) = 0;
  virtual bool hungry() const = 0;
};

class HttpParser {
 public:
  HttpParser();
  bool hungry() const;
  size_t consume(std::unique_ptr<std::string> chunk);
  HttpRequest request() const;
 private:
  std::shared_ptr<HttpRequest> request_;
  std::deque<std::unique_ptr<Parser>> parsers_;
  std::vector<string::StringView> chunks_;
};

class RequestLineMethodParser : public Parser {
 public:
  RequestLineMethodParser(std::shared_ptr<HttpRequest> request);
  bool hungry() const override;
  size_t consume(string::StringView chunk) override;
 private:
  std::shared_ptr<HttpRequest> request_;
  bool hungry_;
  string::StringView token_;
};

class RequestLineUriParser : public Parser {
 public:
  RequestLineUriParser(std::shared_ptr<HttpRequest> request);
  bool hungry() const override;
  size_t consume(string::StringView chunk) override;
 private:
  std::shared_ptr<HttpRequest> request_;
  bool hungry_;
};

class RequestLineProtocolVersionParser : public Parser {
 public:
  RequestLineProtocolVersionParser(std::shared_ptr<HttpRequest> request);
  bool hungry() const override;
  size_t consume(string::StringView chunk) override;
 private:
  std::shared_ptr<HttpRequest> request_;
  bool hungry_;
};


} // namespace parse
} // namespace http
} // namespace ssfs
