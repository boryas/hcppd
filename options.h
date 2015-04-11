#ifndef _OPTS_H
#define _OPTS_H

#include <getopt.h>

#include "util.h"

std::map<std::string, std::string> get_options(int argc, char **argv);
void log_options(std::map<std::string, std::string> options);

#endif //_OPTS_H
