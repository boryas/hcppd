#include "options.h"

using namespace std;

static struct option long_options[] =
{
  {"port", required_argument, nullptr, 'p'},
  {"dynamic_content_server", required_argument, nullptr, 's'},
};

map<string, string> get_options(int argc, char **argv) {
  map<string, string> options;
  int c;
  int option_index = 0;
  while ((c = getopt_long(argc, argv, "p:s:",
                          long_options, &option_index)) != -1) {
    switch(c) {
    case 's':
      options["dynamic_content_server"] = optarg;
      break;
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
