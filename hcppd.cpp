#include "server.h"

using namespace bostack::sock;
using namespace bostack::httpd;

void sig_chld(int sig) {
  int pid;
  int stat;
  pid = wait(&stat);
  std::cout << "child " << pid << " terminated\n";
  return;
}

int main() {
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sig_chld;
  sigaction(SIGCHLD, &sa, NULL);
  HttpServer server;
  server.serve();
}
