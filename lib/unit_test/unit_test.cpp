#include "unit_test.h"

#include <iostream>

namespace ssfs {
namespace unit_test {

Test::~Test() {}

void TestSuite::run() {
  for (auto& t : tests_) {
    try {
      t->run();
    } catch (const AssertionError& e) {
      failures_.push_back({std::move(t), e.reason});
      continue;
    } catch (const std::exception& e) {
      std::cout << "caught an exception in " << t->name
                << ": " << e.what() << "\n";
      continue;
    }
    successes_.push_back(std::move(t));
  }
}

void TestSuite::add(std::unique_ptr<Test> t) {
  tests_.push_back(std::move(t));
}

void TestSuite::displayResults() const {
  for (const auto& t : successes_) {
    std::cout << t->name << "\t.\n";
  }
  for (const auto& t : failures_) {
    std::cout << t.first->name << "\tX\t" << t.second << "\n";
  }
}

} // namespace unit_test
} // namespace ssfs
