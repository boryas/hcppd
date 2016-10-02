#pragma once

#include <memory>
#include <string>

#include "lib/fs.h"

namespace servers {
namespace blog {

enum class ResourceType {
  BLOG_POST,
  CSS,
};

class ResourceError : public std::runtime_error {
 public:
  ResourceError() : std::runtime_error("ResourceError") {}
};

ResourceType resolveResourceType(const std::string& request_uri);

template <class T>
class Resource {
 public:
  Resource (const std::string& request_uri) {
    auto path = t().resolveUri(request_uri);
    lib::fs::Stat s(path);
    if (!s.isRegularFile()) {
      throw ResourceError();
    }
    f_ = std::make_unique<lib::fs::File>(path);
  }
  virtual ~Resource() {}
  std::string get() {
    return f_->read();
  }
 private:
  T& t() { return *static_cast<T*>(this); }
  std::unique_ptr<lib::fs::File> f_;
};

class BlogPost : public Resource<BlogPost> {
 public:
  BlogPost(const std::string& request_uri) :
    Resource<BlogPost>(request_uri) {}
  std::string resolveUri(const std::string& request_uri) const;
};

class Css : public Resource<Css> {
 public:
  Css(const std::string& request_uri) :
    Resource<Css>(request_uri) {}
  std::string resolveUri(const std::string& request_uri) const;
};

} // namespace blog
} // namespace servers
