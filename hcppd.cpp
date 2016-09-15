#include <signal.h>
#include <sys/wait.h>

#include "lib/options.h"
#include "server.h"

using namespace hcppd;
using namespace sock;

void sig_chld(int sig) {
  int stat;
  wait(&stat);
  return;
}

int main(int argc, char **argv) {
  auto options = get_options(argc, argv);
  log_options(options);
  daemon(1, 0);
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sig_chld;
  sigaction(SIGCHLD, &sa, NULL);
  HttpServer server;
  auto port = options.find("port");
  if (port != options.end()) {
    server.port = port->second;
  }
  auto dynamic_content_server = options.find("dynamic_content_server");
  if (dynamic_content_server != options.end()) {
    server.dynamic_content_server = dynamic_content_server->second;
  }
  server.serve();
}
