#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

#include "lib/sock.h"

static const char * const ssfs_srv_usage[] = {
	"usage: ssfs-srv [options]\n Runs the ssfs server",
  "-h|--help        print this message.",
	"-v|--verbose     increase verbosity about performed actions.",
	"-p|--port        TCP port to listen on. (Default 80)",
};

void usage() {
  printf("%s\n", ssfs_srv_usage[0]);
  for (int i = 1; i < 4; ++i) {
    printf("\t%s\n", ssfs_srv_usage[i]);
  }
}

// TODO: should this close sockets on failure?
int ssfs_serve(char *port, sa_family_t family) {
  int sockfd; // socket we bind
  int connfd; // socket returned by accept
  int status;

  sockfd = ssfs_listen(port, family);
  if (sockfd < 0) {
    return sockfd;
  }

  connfd = ssfs_accept(sockfd);
  if (connfd < 0) {
    return connfd;
  }

  printf("Successfully accepted a connection!\n");
  return 0;
}

int main(int argc, char **argv) {
  int c;
  optind = 0;
  
  int verbosity = 0;
  char port[8] = "80";

  while (1) {
    static const struct option long_opts[] = {
      { "help", no_argument, NULL, 'h' },
      { "verbose", no_argument, NULL, 'v' },
      { "port", required_argument, NULL, 'p' },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long(argc, argv, "hvp:", long_opts, NULL);
    if (c < 0)
      break;
    switch(c) {
    case 'h':
      usage();
      return 0;
    case 'v':
      verbosity++;
      break;
    case 'p':
      memset(port, 0, sizeof(port));
      strcpy(port, optarg);
      break;
    }
  }

  printf("Starting ssfs-srv listening on port %s\n", port);
  ssfs_serve(port, AF_INET6);
  return 0;
};
