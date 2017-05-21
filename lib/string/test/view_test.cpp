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

}

int main(int argc, char **argv) {
  ssfs::unit_test::TestSuite test_suite;
  auto at_test = std::make_unique<AtTest>();
  auto at_out_of_bounds_test = std::make_unique<AtOutOfBoundsTest>();
  test_suite.add(std::move(at_test));
  test_suite.add(std::move(at_out_of_bounds_test));
  test_suite.run();
  test_suite.displayResults();
}
