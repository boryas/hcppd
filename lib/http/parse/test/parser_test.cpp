#include <memory>
#include <string>

#include "lib/unit_test.h"
#include "lib/http/request.h"
#include "lib/http/parse/parser.h"

namespace {
class ParserTest : public lib::unit_test::Test {
 public:
  ParserTest() {
    name = "Basic HTTP Parser Test";
  }
  void run() const override {
    auto input = std::make_unique<std::string>("GET / HTTP1.1\n");
    lib::http::parse::HttpParser http_parser;
    http_parser.consume(std::move(input));
    auto req = http_parser.request();
    lib::unit_test::assertEqual(lib::http::HttpMethod::GET, req.method());
    std::string root("/");
    lib::unit_test::assertEqual(root, req.uri());
  }
};
}

int main(int argc, char **argv) {
  lib::unit_test::TestSuite test_suite;
  auto test = std::make_unique<ParserTest>();
  test_suite.add(std::move(test));
  test_suite.run();
  test_suite.displayResults();
}
