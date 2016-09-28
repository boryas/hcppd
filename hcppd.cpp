#include <signal.h>
#include <sys/wait.h>

#include "lib/options.h"
#include "servers/http_fs.h"


void sig_chld(int sig) {
  int stat;
  wait(&stat);
  return;
}

int main(int argc, char **argv) {
  auto options = lib::options::get_options(argc, argv);
  lib::options::log_options(options);
  daemon(1, 0);
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sig_chld;
  sigaction(SIGCHLD, &sa, NULL);
  servers::HttpFsServer server(options["port"]);
  server.serve();
}
