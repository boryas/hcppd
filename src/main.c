#include <error.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

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

int getaddrinfo_for_bind(char *port, sa_family_t family,
                         struct sockaddr_storage *saddr) {
  int status;
  struct addrinfo hints;
  struct addrinfo *addrinfos;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = family;
  status = getaddrinfo(NULL, port, &hints, &addrinfos);
  if (status != 0) {
    error(0, 0, "Failed to create socket address: %s", gai_strerror(status));
    goto out;
  }
  memmove(saddr, (struct sockaddr_storage *)addrinfos->ai_addr, sizeof(*saddr));
out:
  freeaddrinfo(addrinfos);
  return status;
}

int ssfs_setup_listen(char *port, sa_family_t family) {
  int sockfd; // socket we bind
  struct sockaddr_storage local;
  int status;
  int yes = 1;

  sockfd = socket(family, SOCK_STREAM, 0);
  if (sockfd == -1) {
    error(0, errno, "failed to create socket");
    return -errno;
  }

  status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (status) {
    error(0, errno, "failed to set SO_REUSEADDR on socket");
    status = errno;
    goto fail;
  }

  status = getaddrinfo_for_bind(port, family, &local);
  if (status) {
    goto fail;
  }

  if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1) {
    error(0, errno, "failed to bind socket");
    status = errno;
    goto fail;
  }

  if (listen(sockfd, 10) == -1) {
    error(0, errno, "failed to listen on socket");
    status = errno;
    goto fail;
  }

  return sockfd;

fail:
  close(sockfd);
  return -status;
}

int serve(char *port, sa_family_t family) {
  int sockfd; // socket we bind
  int connfd; // socket returned by accept
  struct sockaddr_storage remote;
  socklen_t addrsize = sizeof(remote);
  int status;
  int i;
  int w;

  sockfd = ssfs_setup_listen(port, family);
  if (sockfd < 0) {
    return sockfd;
  }

  connfd = accept(sockfd, (struct sockaddr*)&remote, &addrsize);
  if (connfd == -1) {
    error(0, errno, "failed to accept a connection");
    return errno;
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
  serve(port, AF_INET6);
  return 0;
};
