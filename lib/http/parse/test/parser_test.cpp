#include <memory>
#include <string>

#include "lib/unit_test.h"
#include "lib/http/request.h"
#include "lib/http/parse/parser.h"

namespace {
class BasicParserTest : public ssfs::unit_test::Test {
 public:
  BasicParserTest() {
    name = "Basic HTTP parsing";
  }
  void run() const override {
    auto input = std::make_unique<std::string>("GET / HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;
    http_parser.consume(std::move(input));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
  }
};

class TwoChunkParserTest : public ssfs::unit_test::Test {
 public:
  TwoChunkParserTest() {
    name = "Two chunk HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("GET ");
    auto chunk2 = std::make_unique<std::string>("/ HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;

    // Eat in first chunk, should be able to parse out method
    http_parser.consume(std::move(chunk1));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk2));
    req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
    ssfs::unit_test::assertEqual(false, http_parser.hungry());
  }
};

class ChunkOnDelimiterParserTest : public ssfs::unit_test::Test {
 public:
  ChunkOnDelimiterParserTest() {
    name = "All but the delimiter in a chunk HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("GET");
    auto chunk2 = std::make_unique<std::string>(" / HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;

    // Eat in first chunk, nothing yet
    http_parser.consume(std::move(chunk1));
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk2));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
    ssfs::unit_test::assertEqual(false, http_parser.hungry());
  }
};

class ChunkOnlyDelimiterParserTest : public ssfs::unit_test::Test {
 public:
  ChunkOnlyDelimiterParserTest() {
    name = "Delimiter in its own chunk HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("GET");
    auto chunk2 = std::make_unique<std::string>(" ");
    auto chunk3 = std::make_unique<std::string>("/ HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;

    // Eat in first chunk, nothing yet
    http_parser.consume(std::move(chunk1));
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, should be able to parse out method
    http_parser.consume(std::move(chunk2));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in third chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk3));
    req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
    ssfs::unit_test::assertEqual(false, http_parser.hungry());
  }
};

class TwoChunkTokenTest : public ssfs::unit_test::Test {
 public:
  TwoChunkTokenTest () {
    name = "Two chunks for one token HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("GE");
    auto chunk2 = std::make_unique<std::string>("T / HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;

    // Eat in first chunk, nothing yet
    http_parser.consume(std::move(chunk1));
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk2));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
    ssfs::unit_test::assertEqual(false, http_parser.hungry());

  }
};

class ThreeChunkTokenTest : public ssfs::unit_test::Test {
 public:
  ThreeChunkTokenTest() {
    name = "Three chunks for one token HTTP parsing";
  }
  void run() const override {
    auto chunk1 = std::make_unique<std::string>("G");
    auto chunk2 = std::make_unique<std::string>("E");
    auto chunk3 = std::make_unique<std::string>("T / HTTP1.1\n");
    ssfs::http::parse::HttpParser http_parser;

    // Eat in first chunk, nothing yet
    http_parser.consume(std::move(chunk1));
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in second chunk, still nothing
    http_parser.consume(std::move(chunk2));
    ssfs::unit_test::assertEqual(true, http_parser.hungry());

    // Eat in third chunk, should be able to parse the rest
    http_parser.consume(std::move(chunk3));
    auto req = http_parser.request();
    ssfs::unit_test::assertEqual(ssfs::http::HttpMethod::GET, req.method());
    std::string root("/");
    ssfs::unit_test::assertEqual(root, req.uri());
    ssfs::unit_test::assertEqual(false, http_parser.hungry());

  }
};
}

int main(int argc, char **argv) {
  ssfs::unit_test::TestSuite test_suite;
  auto basic_test = std::make_unique<BasicParserTest>();
  auto two_chunk_test = std::make_unique<TwoChunkParserTest>();
  auto chunk_on_delimiter_test = std::make_unique<ChunkOnDelimiterParserTest>();
  auto chunk_only_delimiter_test =
    std::make_unique<ChunkOnlyDelimiterParserTest>();
  auto two_chunk_token_test = std::make_unique<TwoChunkTokenTest>();
  auto three_chunk_token_test = std::make_unique<ThreeChunkTokenTest>();
  test_suite.add(std::move(basic_test));
  test_suite.add(std::move(two_chunk_test));
  test_suite.add(std::move(chunk_on_delimiter_test));
  test_suite.add(std::move(chunk_only_delimiter_test));
  test_suite.add(std::move(two_chunk_token_test));
  test_suite.add(std::move(three_chunk_token_test));
  test_suite.run();
  test_suite.displayResults();
}
