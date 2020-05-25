#include "sock.h"

#include <error.h> // error
#include <errno.h> // errno
#include <netdb.h> // getaddrinfo
#include <string.h> // memset
#include <unistd.h> // close

static int getaddrinfo_for_bind(char *port, sa_family_t family,
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

int ssfs_listen(char *port, sa_family_t family, struct ssfs_conn *conn) {
  struct sockaddr_storage local;
  int status;
  int yes = 1;

  conn->listenfd = socket(family, SOCK_STREAM, 0);
  if (conn->listenfd == -1) {
    error(0, errno, "failed to create socket");
    return -errno;
  }

  status =
      setsockopt(conn->listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (status) {
    error(0, errno, "failed to set SO_REUSEADDR on socket");
    status = errno;
    goto fail;
  }

  status = getaddrinfo_for_bind(port, family, &local);
  if (status) {
    goto fail;
  }

  if (bind(conn->listenfd, (struct sockaddr *)&local, sizeof(local)) == -1) {
    error(0, errno, "failed to bind socket");
    status = errno;
    goto fail;
  }

  if (listen(conn->listenfd, 10) == -1) {
    error(0, errno, "failed to listen on socket");
    status = errno;
    goto fail;
  }

  return 0;

fail:
  close(conn->listenfd);
  return -status;
}

int ssfs_accept(struct ssfs_conn *conn) {
  struct sockaddr_storage remote;
  socklen_t addrsize = sizeof(remote);
  int status;

  conn->connfd = accept(conn->listenfd, (struct sockaddr *)&remote, &addrsize);
  if (conn->connfd == -1) {
    error(0, errno, "failed to accept a connection");
    return -errno;
  }

  return 0;
}
