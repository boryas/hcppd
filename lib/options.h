#pragma once

#include <getopt.h>

#include <map>
#include <string>

std::map<std::string, std::string> get_options(int argc, char **argv);
void log_options(std::map<std::string, std::string> options);
