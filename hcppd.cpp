#include "options.h"
#include "server.h"

using namespace hcppd;
using namespace sock;
using namespace std;

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
  server.serve();
}
