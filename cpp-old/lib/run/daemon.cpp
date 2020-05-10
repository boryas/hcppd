#include "daemon.h"

#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace ssfs {
namespace run {

void sigchldHandler(int sig) {
  int stat;
  auto pid = ::wait(&stat);
  syslog(LOG_INFO, "child %d exited", pid);
  return;
}

void daemonize() {
  ::daemon(1, 0);
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sigchldHandler;
  sigaction(SIGCHLD, &sa, nullptr);
}

} // namespace run
} // namespace ssfs
