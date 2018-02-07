/*
 * unix_server.cpp
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */
#include <poll.h>
#include <iostream>

#include "unix_server.h"

static const int kBuffLen = 1024;  // размер буфера
static const int kPollTimeout = 1000;  // размер буфера

UnixServer::UnixServer(const std::string& path)
    : socket_filename_(path),
      server_(-1){
  Create();
}

UnixServer::~UnixServer() {
  unlink(socket_filename_.c_str());
  close(server_);
  std::cout << "UNIX Server socket unlinked and closed." << std::endl;
}

void UnixServer::Create() {
  memset(&server_addr_, 0, sizeof(server_addr_));
  server_addr_.sun_family = AF_UNIX;
  strncpy(server_addr_.sun_path, socket_filename_.c_str(),
          sizeof(server_addr_.sun_path) - 1); // should be limited to about 104 characters, system dependent

  // create socket
  server_ = socket(PF_UNIX, SOCK_DGRAM, 0);
  if (server_ < 0) {
    perror("Can't create file descriptor for the new socket.");
    exit(-1);
  }

  // call bind to associate the socket with the UNIX file system
  if (bind(server_, (sockaddr *) &server_addr_,
           sizeof(server_addr_)) < 0) {
    perror("Can't bind socket and server_addr.");
    exit(-1);
  }

  std::cout << "UNIX Server started on" + socket_filename_ << std::endl;
}

void UnixServer::Loop() {
  while(true) {
    std::string request = "";
    GetRequest(request);
    if(request.empty()) {
      continue;
    } else if(request.compare("Client is closed.\n")==0) {
      std::cout << request;
      break;
    }
    std::cout << request;
  }
}

void UnixServer::GetRequest(std::string& request) {
  // read until we get a newline
  char buf[kBuffLen]= { 0 };

  struct pollfd fd;
  fd.fd = server_;
  fd.events = POLLIN;

  socklen_t len = sizeof(struct sockaddr_un);

  int ret = poll(&fd, 1, kPollTimeout);
  if (ret < 0 || fd.revents > 1) {
    perror("Socket error. poll()");
    return;
  } else if (ret == 0) {
    return;
  }

  int nread = recvfrom(server_, buf, kBuffLen, 0,
                       (struct sockaddr *) &server_addr_, &len);
  if (nread <= 0) {
    perror("Socket error, while receiving data from socket. recvfrom()");
    return;
  }
  // be sure to use append in case we have binary data
  request.append(buf, nread);

  if(request.find("\n") == std::string::npos) {
    request += "\n";
  }
}
