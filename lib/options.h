#pragma once

#include <getopt.h>

#include <map>
#include <string>

namespace lib {
namespace options {

std::map<std::string, std::string> get_options(int argc, char **argv);
void log_options(std::map<std::string, std::string> options);

} // namespace options
} // namespace lib
