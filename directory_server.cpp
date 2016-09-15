#include <dirent.h>
#include <syslog.h>
#include <sys/stat.h>

#include <fstream>
#include <sstream>

#include "http_response.h"
#include "lib/options.h"
#include "lib/socket.h"

const std::string PROTOCOL_VERSION = "HTTP/1.1";

hcppd::HttpResponse respond(int status,
                            const std::string& reason,
                            const std::string& message) {
  hcppd::HttpResponse response;
  response.status_line.protocol_version = PROTOCOL_VERSION;
  response.status_line.status_code = status;
  response.status_line.reason_phrase = reason;
  response.message = message;
  return response;
}

hcppd::HttpResponse handle(std::string uri) {
  syslog(LOG_INFO, "Responding to request for: %s", uri.c_str());
  struct stat st;
  if (stat(uri.c_str(), &st) == -1) {
    if (errno == ENOENT) {
      syslog(LOG_ERR, "URI not found: %s: %m", uri.c_str());
      return respond(404, "Not Found!", "");
    }
    syslog(LOG_ERR, "Error stat-ing directory %s: %m", uri.c_str());
    return respond(500, "Error stat-ing directory", "");
  }
  std::stringstream res_ss;
  if (st.st_mode & S_IFDIR) {
    if (uri.at(uri.size()-1) != '/') {
      uri.push_back('/');
    }
    DIR* d;
    if ((d = opendir(uri.c_str())) == NULL) {
      syslog(LOG_ERR, "Error opening directory %s: %m", uri.c_str());
      return respond(500, "Error opening directory", "");
    }
    struct dirent* dir;
    while ((dir = readdir(d))) {
      std::string d_name(dir->d_name);
      if (d_name == "." || d_name == "..") {
        continue;
      }
      struct stat st2;
      std::string d_uri;
      std::stringstream ss;
      ss << uri << d_name;
      d_uri = ss.str();
      if (stat(d_uri.c_str(), &st2) == -1) {
        syslog(LOG_ERR, "Could not stat file in directory; %m");
        continue;
      }
      if (st.st_mode & S_IFDIR) {
        res_ss << "<a href=" << d_uri << ">"
               << d_uri << "</a><br>" << std::endl;
      }
    }
  }
  else if (st.st_mode & S_IFREG) {
    std::ifstream fin{uri, std::ifstream::in};
    std::string line;
    while (getline(fin, line)) {
      res_ss << line << " <br>" << std::endl;
    }
  }
  else {
    res_ss << "somethin' else";
  }
  return respond(200, "OK", res_ss.str());
}

int main(int argc, char **argv) {
  std::unique_ptr<lib::sock::Socket> sock_;
  unlink("/var/run/hcppd/sock");
  sock_.reset(new lib::sock::Socket("/var/run/hcppd/sock", AF_LOCAL));
  sock_->Bind();
  sock_->Listen();
  for ( ; ; ) {
    sock_->Accept();
    int pid = fork();
    if (pid < 0) {
      // error
      syslog(LOG_WARNING, "failed to fork child to handle connection");
    } else if (pid == 0) {
      // child
      close(sock_->getListenFd());
      std::string uri;
      sock_->Read(&uri);
      sock_->Write(handle(uri).format());
      exit(0);
    }
    else {
      // parent
      close(sock_->getConnFd());
    }
  }
}
