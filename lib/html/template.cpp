#include "template.h"

#include "lib/fs/fs.h"

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

std::unique_ptr<HtmlTemplate> HtmlTemplate::fromFile(const std::string& filename) {
 auto t = std::make_unique<std::string>(ssfs::fs::readFile(filename));
 return std::make_unique<HtmlTemplate>(std::move(t));
}

} // namespace html
} // namespace ssfs
