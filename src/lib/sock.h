#ifndef _SSFS_SOCK_H
#define _SSFS_SOCK_H

#include <sys/socket.h> // sa_family_t

struct ssfs_conn {
  int listenfd;
  int connfd;
};

int ssfs_listen(char *port, sa_family_t family, struct ssfs_conn *conn);
int ssfs_accept(struct ssfs_conn *conn);

#endif // _SSFS_SOCK_H
