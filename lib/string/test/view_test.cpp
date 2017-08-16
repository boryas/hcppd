#include <iostream>
#include <memory>
#include <string>

#include "lib/string/view.h"
#include "lib/unit_test/unit_test.h"

namespace {
class AtTest : public ssfs::unit_test::Test {
 public:
  AtTest() {
    name = "StringView::at()";
  }
  void run() const override {
    auto str = std::make_shared<std::string>("lol");
    auto view = ssfs::string::StringView(str);
    ssfs::unit_test::assertEqual('l', view.at(0));
    ssfs::unit_test::assertEqual('o', view.at(1));
    ssfs::unit_test::assertEqual('l', view.at(2));
  }
};

class AtOutOfBoundsTest : public ssfs::unit_test::Test {
 public:
  AtOutOfBoundsTest() {
    name = "StringView::at() with out of bounds index";
  }
  void run() const override {
    auto str = std::make_shared<std::string>("lol");
    auto view = ssfs::string::StringView(str);
    auto bad_idxs = {3, 5, 42};
    for (const auto& idx: bad_idxs) {
      try {
        view.at(idx);
      } catch (const ssfs::string::StringViewError& e) {
        continue;
      }
      ssfs::unit_test::assertEqual(true, false);
    }
  }
};

class SubTest : public ssfs::unit_test::Test {
 public:
  SubTest() {
    name = "StringView::sub(0, i) i < size";
  }
  void run() const override {
    auto sp = std::make_shared<std::string>("lol");
    auto sv = ssfs::string::StringView(sp);
    for (size_t i = 0; i <= sv.size(); ++i) {
      auto subi = sv.sub(0, i);
      ssfs::unit_test::assertEqual(subi.str(), sp->substr(0, i));
    };
  }
};

class OperatorPlusTest : public ssfs::unit_test::Test {
 public:
  OperatorPlusTest() {
    name = "StringView::operator+";
  }
  void run() const override {
    auto s1 = std::make_shared<std::string>("");
    auto s2 = std::make_shared<std::string>("lol");
    auto s3 = std::make_shared<std::string>("lmao");

    auto sv1 = ssfs::string::StringView(s1);
    auto sv2 = ssfs::string::StringView(s2);
    auto sv3 = ssfs::string::StringView(s3);

    auto p1 = sv1 + sv2;
    auto p2 = sv2 + sv1;
    auto p3 = sv2 + sv3;

    ssfs::unit_test::assertEqual(p1.str(), *s2);
    ssfs::unit_test::assertEqual(p2.str(), *s2);
    ssfs::unit_test::assertEqual(p3.str(), *s2 + *s3);
  }
};

class OperatorPlusEqualTest : public ssfs::unit_test::Test {
 public:
  OperatorPlusEqualTest() {
    name = "StringView::operator+=";
  }
  void run() const override {
    auto s1 = std::make_shared<std::string>("");
    auto s2 = std::make_shared<std::string>("lol");
    auto s3 = std::make_shared<std::string>("lmao");

    auto sv1 = ssfs::string::StringView(s1);
    auto sv2 = ssfs::string::StringView(s2);
    auto sv3 = ssfs::string::StringView(s3);

    sv1 += sv2;
    sv2 += sv3;

    ssfs::unit_test::assertEqual(sv1.str(), *s2);
    ssfs::unit_test::assertEqual(sv2.str(), *s2 + *s3);
  }
};

}

int main(int argc, char **argv) {
  ssfs::unit_test::TestSuite test_suite;
  auto at_test = std::make_unique<AtTest>();
  auto at_out_of_bounds_test = std::make_unique<AtOutOfBoundsTest>();
  auto sub_test = std::make_unique<SubTest>();
  auto plus_test = std::make_unique<OperatorPlusTest>();
  auto plus_equal_test = std::make_unique<OperatorPlusEqualTest>();
  test_suite.add(std::move(at_test));
  test_suite.add(std::move(at_out_of_bounds_test));
  test_suite.add(std::move(sub_test));
  test_suite.add(std::move(plus_test));
  test_suite.add(std::move(plus_equal_test));
  test_suite.run();
  test_suite.displayResults();
}
