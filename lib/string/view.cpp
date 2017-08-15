#include "view.h"

namespace ssfs {
namespace string {

StringView::StringView(std::shared_ptr<std::string> str) :
    StringView(str, 0, str->size()) {}

StringView::StringView(std::shared_ptr<std::string> str, 
                       size_t start) : StringView(str, start, str->size()) {}

StringView::StringView(std::shared_ptr<std::string> str,
                       size_t start,
                       size_t end) :
    str_(str),
    start_(start),
    end_(end) {
  if (start_ < 0) {
    throw StringViewError("negative start index");
  }
  if (start_ > end_) {
    throw StringViewError("start index past end index");
  }
  if (size() < str_->size()) {
    throw StringViewError("end index past end of string");
  }
}

size_t StringView::size() const {
  return end_ - start_;
}

char StringView::at(size_t pos) const {
  if (start_ + pos >= end_) {
    throw StringViewError("at index past end of string");
  }
  return str_->at(start_ + pos);
}

std::string StringView::str() const {
  return str_->substr(start_, end_);
}

StringView StringView::substr(size_t start) {
  return StringView(str_, start_ + start);
}

StringView StringView::substr(size_t start, size_t count) {
  return StringView(str_, start_ + start, start_ + count);
}

StringView StringView::operator+(const StringView& other) {
  StringView ret(*this);
  ret += other;
  return ret;
}

StringView StringView::operator+=(const StringView& other) {
  str_ = std::make_shared<std::string>(str() + other.str());
  start_ = 0;
  end_ = str_->size();
  return *this;
}

} // namespace string
} // namespace ssfs
