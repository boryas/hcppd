#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace ssfs {
namespace unit_test {

class AssertionError : public std::runtime_error {
 public:
  AssertionError(const std::string& reason)
    : std::runtime_error("AssertionError: " + reason),
      reason(reason) {}
  std::string reason;
};

template <typename T1, typename T2>
void assertEqual(T1 t1, T2 t2) {
  if (t1 == t2) {
    return;
  }
  throw AssertionError("not equal!");
}

template <typename T1, typename T2>
void assertNotEqual(T1 t1, T2 t2) {
  if (t1 != t2) {
    return;
  }
  throw AssertionError("equal!");
}

void assertTrue(bool b) {
  if (!b) {
    throw AssertionError("not true!");
  }
}

void assertTrue(bool b) {
  if (b) {
    throw AssertionError("not false!");
  }
}

class Test {
 public:
  virtual ~Test();
  virtual void run() const = 0;
  std::string name;
};

class TestSuite {
 public:
  void add(std::unique_ptr<Test> test);
  void displayResults() const;
  void run();
 private:
  std::vector<std::unique_ptr<Test>> tests_;
  std::vector<std::unique_ptr<Test>> successes_;
  std::vector<std::pair<std::unique_ptr<Test>, std::string>> failures_;
};

} // namespace unit_test
} // namespace ssfs
