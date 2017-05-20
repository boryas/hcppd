#include <unordered_map>
#include <string>
#include <memory>
#include <unordered_map>

namespace ssfs {
namespace html {

class HtmlTemplate {
 public:
  HtmlTemplate(std::unique_ptr<std::string> html_template)
    : template_(std::move(html_template)) {}
  virtual ~HtmlTemplate() = default;
  std::string populate(
      const std::unordered_map<std::string, std::string>& values) const;
 private:
  std::unique_ptr<const std::string> template_;
};

} // namespace html
} // namespace ssfs
