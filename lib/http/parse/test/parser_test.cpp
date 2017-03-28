#include <memory>
#include <string>

#include "lib/unit_test.h"
#include "lib/http/request.h"
#include "lib/http/parse/parser.h"

namespace {
class BasicParserTest : public lib::unit_test::Test {
 public:
  BasicParserTest() {
    name = "Basic HTTP parsing";
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

class TwoChunkParserTest : public lib::unit_test::Test {
 public:
  TwoChunkParserTest() {
    name = "Two chunk HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("GET ");
    auto chunk2 = std::make_unique<std::string>("/ HTTP1.1\n");
    lib::http::parse::HttpParser http_parser;

    // Eat in first chunk, should be able to parse out method
    http_parser.consume(std::move(chunk1));
    auto req = http_parser.request();
    lib::unit_test::assertEqual(lib::http::HttpMethod::GET, req.method());
    lib::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk2));
    req = http_parser.request();
    lib::unit_test::assertEqual(lib::http::HttpMethod::GET, req.method());
    std::string root("/");
    lib::unit_test::assertEqual(root, req.uri());
  }
};

}

int main(int argc, char **argv) {
  lib::unit_test::TestSuite test_suite;
  auto basic_test = std::make_unique<BasicParserTest>();
  auto two_chunk_test = std::make_unique<TwoChunkParserTest>();
  test_suite.add(std::move(basic_test));
  test_suite.add(std::move(two_chunk_test));
  test_suite.run();
  test_suite.displayResults();
}
