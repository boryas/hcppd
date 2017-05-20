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
  chunks_.emplace_back(ch);
  size_t i = 0;
  while (i < ch->size() && hungry()) {
    auto s = std::make_shared<std::string>(ch->substr(i));
    i += parsers_.front()->consume(s);
    if (!parsers_.front()->hungry()) {
      parsers_.pop_front();
    }
  }
  return i;
}

RequestLineMethodParser::RequestLineMethodParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineMethodParser::consume(std::shared_ptr<std::string> chunk) {
  syslog(LOG_INFO, "RequestLineMethodParser consume %s", chunk->c_str());
  for (size_t i = 0; i < chunk->size(); ++i) {
    char c = chunk->at(i);
    if (c == ' ') {
      std::string token;
      if (!token_) {
        token = chunk->substr(0, i);
      } else {
        token = *token_ + chunk->substr(0, i);
      }
      syslog(LOG_INFO, token.c_str());
      if (token == "HEAD") {
        request_->request_line.method = HttpMethod::HEAD;
      } else if (token == "GET") {
        request_->request_line.method = HttpMethod::GET;
      } else if (token == "POST") {
        request_->request_line.method = HttpMethod::POST;
      } else {
        throw ParserError("Invalid method " + token);
      }
      hungry_ = false;
      return i+1;
    }
  }
  if (!token_) {
    token_ = chunk;
  } else {
    *token_ += *chunk;
  }
  return chunk->size();
}

bool RequestLineMethodParser::hungry() const {
  return hungry_;
}

RequestLineUriParser::RequestLineUriParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineUriParser::consume(std::shared_ptr<std::string> chunk) {
  syslog(LOG_INFO, "RequestLineUriParser consume %s", chunk->c_str());
  size_t token_start = 0;
  for (size_t i = 0; i < chunk->size(); ++i) {
    char c = chunk->at(i);
    if (c == ' ') {
      auto token = chunk->substr(token_start, i-token_start);
      syslog(LOG_INFO, token.c_str());
      request_->request_line.uri = token;
      hungry_ = false;
      return i+1;
    }
  }
  return chunk->size();
}

bool RequestLineUriParser::hungry() const {
  return hungry_;
}

RequestLineProtocolVersionParser::RequestLineProtocolVersionParser(
    std::shared_ptr<HttpRequest> request) : request_(request), hungry_(true) {}

size_t RequestLineProtocolVersionParser::consume(
    std::shared_ptr<std::string> chunk) {
  syslog(LOG_INFO, "RequestLineProtocolVersionParser consume %s", chunk->c_str());
  size_t token_start = 0;
  for (size_t i = 0; i < chunk->size(); ++i) {
    char c = chunk->at(i);
    if (c == '\n') {
      auto token = chunk->substr(token_start, i-token_start);
      syslog(LOG_INFO, token.c_str());
      request_->request_line.protocol_version = token;
      hungry_ = false;
      return i+1;
    }
  }
  return chunk->size();
}

bool RequestLineProtocolVersionParser::hungry() const {
  return hungry_;
}

} // namespace parse
} // namespace http
} // namespace ssfs
