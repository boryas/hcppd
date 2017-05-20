#pragma once

#include <getopt.h>

#include <unordered_map>
#include <string>

namespace ssfs {
namespace options {

std::unordered_map<std::string, std::string> get_options(int argc, char **argv);
void log_options(std::unordered_map<std::string, std::string> options);

} // namespace options
} // namespace ssfs
