#pragma once

#include <unordered_map>
#include <memory>
#include <string>

namespace ssfs {
namespace html {

/*
 * Class that represents a map from resources to templates
 * to use for rendering them as html.
 */
class UriMap {
 private:
  std::unordered_map<std::string, std::unique_ptr<HtmlTemplate>> uri_map_;
};

} // namespace html
} // namespace ssfs
