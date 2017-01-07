#include "parser.h"

#include <sstream>

namespace lib {
namespace http {
namespace parse {

bool Parser::hungry() const {
  return state_ != ParserState::DONE;
}

void Parser::consume(std::unique_ptr<std::string> chunk) {
  auto cur = std::shared_ptr<std::string>(std::move(chunk));
  chunks_.emplace_back(cur);
  size_t token_start = 0;
  for (size_t i = 0; i < cur->size(); ++i) {
    if (state_ == ParserState::DONE) {
      return;
    }
    char c = cur->at(i);
    if (c == ' ' || c == '\n') {
      auto token = cur->substr(token_start, i-token_start);
      syslog(LOG_INFO, token.c_str());
      consumeToken(token);
      token_start = i+1;
    }
  }
}

void Parser::consumeToken(const std::string& token) {
  switch (state_) {
    case ParserState::REQUEST_LINE_METHOD:
      if (token == "HEAD") {
        req_.request_line.method = HttpMethod::HEAD;
      }
      if (token == "GET") {
        req_.request_line.method = HttpMethod::GET;
      }
      if (token == "POST") {
        req_.request_line.method = HttpMethod::POST;
      }
      state_ = ParserState::REQUEST_LINE_URI;
      break;
    case ParserState::REQUEST_LINE_URI:
      req_.request_line.uri = token;
      state_ = ParserState::REQUEST_LINE_PROT;
      break;
    case ParserState::REQUEST_LINE_PROT:
      req_.request_line.protocol_version = token;
      state_ = ParserState::DONE;
      break;
    default:
      throw ParserError("Bad ParserState");
  }
}

HttpRequest Parser::request() const {
  if (hungry()) {
    throw ParserError("Request not fully parsed yet");
  }
  return req_;
}

} // namespace parse
} // namespace http
} // namespace lib
