#include "resource.h"

namespace servers {
namespace blog {

auto constexpr POST_DIR = "/home/bb/blog/posts";
auto constexpr CSS_DIR = "/home/bb/blog/css";

ResourceType resolveResourceType(const std::string& request_uri) {
  auto dot_pos = request_uri.rfind('.');
  if (dot_pos == std::string::npos) {
    return ResourceType::BLOG_POST;
  }
  auto extension = request_uri.substr(dot_pos);
  if (extension == ".css") {
    return ResourceType::CSS;
  }
  throw ResourceError();
}

std::string BlogPost::resolveUri(const std::string& request_uri) const {
  return POST_DIR + request_uri;
};

std::string Css::resolveUri(const std::string& request_uri) const {
  return CSS_DIR + request_uri;
};

} // namespace blog
} // namespace servers
