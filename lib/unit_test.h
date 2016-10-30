#pragma once

#include <pair>
#include <stdexcept>
#include <string>
#include <vector>

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
  const std::string name;
};

class TestSuite {
 public:
  void add();
  void displayResults() const;
  void run() const;
 private:
  std::vector<Test> tests_;
  std::vector<Test> successes_;
  std::vector<std::pair<Test, std::string>> failures_;
};
