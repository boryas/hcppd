#include "parser.h"

#include <sstream>
#include <unordered_set>

namespace ssfs {
namespace http {
namespace parse {

HttpParser::HttpParser() {
  request_ = std::make_shared<HttpRequest>();
  auto mp = std::make_unique<RequestLineMethodParser>(request_);
  auto up = std::make_unique<RequestLineUriParser>(request_);
  auto pvp = std::make_unique<RequestLineProtocolVersionParser>(request_);
  parsers_.push_back(std::move(mp));
  parsers_.push_back(std::move(up));
  parsers_.push_back(std::move(pvp));
}

bool HttpParser::hungry() const {
  return !parsers_.empty() && parsers_.back()->hungry();
}

HttpRequest HttpParser::request() const {
  return *request_;
}

size_t HttpParser::consume(std::unique_ptr<std::string> chunk) {
  syslog(LOG_INFO, "HttpParser consume %s", chunk->c_str());
  auto ch = std::make_shared<std::string>(*chunk.release());
  chunks_.push_back(ch);
  size_t i = 0;
  while (i < ch->size() && hungry()) {
    i += parsers_.front()->consume(string::StringView(ch, i));
    if (!parsers_.front()->hungry()) {
      parsers_.pop_front();
    }
  }
  return i;
}

RequestLineMethodParser::RequestLineMethodParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineMethodParser::consume(string::StringView chunk) {
  syslog(LOG_INFO, "RequestLineMethodParser consume %s", chunk.str().c_str());
  for (size_t i = 0; i < chunk.size(); ++i) {
    char c = chunk.at(i);
    if (c == ' ') {
      string::StringView token = token_ + chunk.sub(0, i);
      auto str = token.str();
      syslog(LOG_INFO, str.c_str());
      if (str == "HEAD") {
        request_->request_line.method = HttpMethod::HEAD;
      } else if (str == "GET") {
        request_->request_line.method = HttpMethod::GET;
      } else if (str == "POST") {
        request_->request_line.method = HttpMethod::POST;
      } else {
        throw ParserError("Invalid method " + str);
      }
      hungry_ = false;
      return i+1;
    }
  }
  token_ += chunk;
  return chunk.size();
}

bool RequestLineMethodParser::hungry() const {
  return hungry_;
}

RequestLineUriParser::RequestLineUriParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineUriParser::consume(string::StringView chunk) {
  syslog(LOG_INFO, "RequestLineUriParser consume %s", chunk.str().c_str());
  size_t token_start = 0;
  for (size_t i = 0; i < chunk.size(); ++i) {
    char c = chunk.at(i);
    if (c == ' ') {
      auto token = chunk.substr(token_start, i-token_start);
      syslog(LOG_INFO, token.c_str());
      request_->request_line.uri = token;
      hungry_ = false;
      return i+1;
    }
  }
  return chunk.size();
}

bool RequestLineUriParser::hungry() const {
  return hungry_;
}

RequestLineProtocolVersionParser::RequestLineProtocolVersionParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineProtocolVersionParser::consume(
    string::StringView chunk) {
  syslog(LOG_INFO, "RequestLineProtocolVersionParser consume %s", chunk.str().c_str());
  size_t token_start = 0;
  for (size_t i = 0; i < chunk.size(); ++i) {
    char c = chunk.at(i);
    if (c == '\n') {
      auto token = chunk.substr(token_start, i-token_start);
      syslog(LOG_INFO, token.c_str());
      request_->request_line.protocol_version = token;
      hungry_ = false;
      return i+1;
    }
  }
  return chunk.size();
}

bool RequestLineProtocolVersionParser::hungry() const {
  return hungry_;
}

} // namespace parse
} // namespace http
} // namespace ssfs
