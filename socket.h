#ifndef _SOCKET_H
#define _SOCKET_H
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "util.h"

#define MAXLINE 4096

namespace sock {

class Sockaddr {
public:
  Sockaddr(const std::string& service, sa_family_t family);
  struct sockaddr *sockaddr() const;
  socklen_t size() const;
private:
  struct sockaddr_storage sockaddr_;
};

class Socket {
public:
  Socket(const std::string& service);
  Socket(const std::string& service, sa_family_t family);
  ~Socket();
  int Bind();
  int Listen();
  int Accept();
  int Connect(const Sockaddr& addr);
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
  std::unique_ptr<Sockaddr> servaddr_;
  std::unique_ptr<Sockaddr> cliaddr_;
  std::string service_;
  int connfd_;
  int listenfd_;
  int sockerr_;
  socklen_t clilen_;
  sa_family_t family_;
  char buffer_[MAXLINE];
};

}
#endif //_SOCKET_H
