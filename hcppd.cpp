#include "server.h"

using namespace sock;
using namespace hcppd;

void sig_chld(int sig) {
  int stat;
  wait(&stat);
  return;
}

int main() {
  daemon(1, 0);
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sig_chld;
  sigaction(SIGCHLD, &sa, NULL);
  HttpServer server;
  server.serve();
}
