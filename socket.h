#ifndef _SOCKET_H
#define _SOCKET_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "util.h"

#define MAXLINE 4096

namespace bostack {
namespace sock {

class Sockaddr {
public:
  Sockaddr();
  struct sockaddr *sockaddr();
  socklen_t size();
private:
  struct sockaddr_in sockaddr_;
};

// TODO: Decide to either generalize this or make it more explicitly
// a IPv4 TCP socket
class Socket {
public:
  Socket();
  ~Socket();
  int Bind();
  int Listen();
  int Accept(socklen_t *clilen);
  int Writen(const char *msg, size_t n);
  int Write(const std::string& msg);
  int Read(std::string *msg);

  int getListenFd() {
    return listenfd_;
  }
  int getConnFd() {
    return connfd_;
  }

private:
  Sockaddr servaddr_;
  Sockaddr cliaddr_;
  int connfd_;
  int listenfd_;
  int sockerr_;
  char buffer_[MAXLINE];
};

}
}
#endif //_SOCKET_H
