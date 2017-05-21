#pragma once

#include <memory>
#include <string>
#include <stdexcept>

namespace ssfs {
namespace string {

class StringViewError : public std::runtime_error {
 public:
  StringViewError(const std::string& reason)
    : std::runtime_error("StringViewError: " + reason) {}
};

class StringView {
 public:
  StringView(std::shared_ptr<std::string> str);
  StringView(std::shared_ptr<std::string> str, size_t start, size_t end);
  size_t size() const;
  char at(size_t pos) const;
  std::string str() const;
 private:
  const std::shared_ptr<std::string> str_;
  size_t start_;
  size_t end_;
};

} // namespace string
} // namespace ssfs
