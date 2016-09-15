#include "options.h"

#include <syslog.h>

static struct option long_options[] =
{
  {"port", required_argument, nullptr, 'p'},
};

std::map<std::string, std::string> get_options(int argc, char **argv) {
  std::map<std::string, std::string> options;
  int c;
  int option_index = 0;
  while ((c = getopt_long(argc, argv, "p:",
                          long_options, &option_index)) != -1) {
    switch(c) {
    case 'p':
      options["port"] = optarg;
      break;
    }
  }
  return options;
}

void log_options(std::map<std::string, std::string> options) {
  for (const auto& option : options) {
     syslog(LOG_INFO, "FLAG %s: %s", option.first.c_str(), option.second.c_str());
  }
}
