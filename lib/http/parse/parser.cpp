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
  syslog(LOG_INFO, cur->c_str());
  std::stringstream ss(*cur);
  std::string raw_rl;
  std::getline(ss, raw_rl, '\n');
  syslog(LOG_INFO, raw_rl.c_str());
  HttpRequestLine rl(raw_rl);
  req_.request_line = rl;
  state_ = ParserState::DONE;
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
