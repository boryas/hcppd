#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lib {
namespace unit_test {

class AssertionError : public std::runtime_error {
 public:
  AssertionError(const std::string& reason)
    : std::runtime_error("AssertionError: " + reason),
      reason(reason) {}
  std::string reason;
};

template <typename T>
void assertEqual(T t1, T t2) {
  if (t1 != t2) {
    throw AssertionError("not equal!");
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
} // namespace lib
