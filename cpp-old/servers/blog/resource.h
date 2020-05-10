#pragma once

#include <memory>
#include <string>

#include "lib/fs/fs.h"

namespace servers {
namespace blog {

enum class ResourceType {
  BLOG_POST,
  CSS,
};

class ResourceError : public std::runtime_error {
 public:
  ResourceError(const std::string& reason) 
    : std::runtime_error("ResourceError: " + reason) {}
};

ResourceType resolveResourceType(const std::string& request_uri);

template <class T>
class Resource {
 public:
  Resource (const std::string& request_uri) {
    auto path = t().resolveUri(request_uri);
    ssfs::fs::Stat s(path);
    if (!s.isRegularFile()) {
      throw ResourceError("Resource is not a regular file");
    }
    f_ = std::make_unique<ssfs::fs::File>(path);
  }
  virtual ~Resource() {}
  std::string get() {
    return f_->read();
  }
 private:
  T& t() { return *static_cast<T*>(this); }
  std::unique_ptr<ssfs::fs::File> f_;
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
