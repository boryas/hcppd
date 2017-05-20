#include "html.h"

namespace ssfs {
namespace html {

std::string HtmlTemplate::populate(
    const std::unordered_map<std::string, std::string>& values) const {
  auto filled_in = *template_;
  for (const auto& kv : values) {
    std::size_t found;
    auto needle = "{{" + kv.first + "}}";
    while ((found = filled_in.find(needle)) != std::string::npos) {
      filled_in.replace(found, needle.length(), kv.second);
    }
  }
  return filled_in;
}

} // namespace html
} // namespace ssfs
