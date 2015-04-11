#include "options.h"

using namespace std;

static struct option long_options[] =
{
  {"dynamic_content_server", required_argument, nullptr, 'd'}
};

map<string, string> get_options(int argc, char **argv) {
  map<string, string> options;
  int c;
  int option_index = 0;
  while ((c = getopt_long(argc, argv, "d:",
                          long_options, &option_index)) != -1) {
    switch(c) {
     case 'd':
       options["dynamic_content_server"] = optarg;
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
