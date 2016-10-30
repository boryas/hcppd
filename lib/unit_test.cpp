#include "unit_test.h"

#include <iostream>

void TestSuite::run() const {
  for (const auto& t : tests_) {
    try {
      t.run();
      successes_.push_back(t);
    } catch (const AssertionError& e) {
      failures_.push_back({t, e.reason});
    }
  }
}

void TestSuite::add(const Test& t) {
  tests_.push_back(t);
}

void TestSuite::displayResults() const {
  for (const auto& t : successes_) {
    std::cout << t.name << "\t.\n";
  }
  for (const auto& t : failures_) {
    std::cout << t.first.name << "\tX\t" << t.second << "\n";
  }
}
