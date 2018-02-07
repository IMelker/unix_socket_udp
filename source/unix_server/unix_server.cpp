/*
 * unix_server.cpp
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>

#include "unix_server.h"

static const int kBuffLen = 1024;  // buffer length
static const int kPollTimeout = 1000;  // socket poll timeout

UnixServer::UnixServer(const std::string& path)
    : socket_filename_(path),
      server_(-1) {
  Create();
}

UnixServer::~UnixServer() {
  unlink(socket_filename_.c_str());
  close(server_);
  std::cout << "UNIX Server socket unlinked and closed." << std::endl;
}

void UnixServer::Create() {
  // fill sockaddr_un
  memset(&server_addr_, 0, sizeof(server_addr_));
  server_addr_.sun_family = AF_UNIX;
  strncpy(server_addr_.sun_path, socket_filename_.c_str(),
          sizeof(server_addr_.sun_path) - 1);  // should be limited to about 104 characters, system dependent

  // create socket
  server_ = socket(PF_UNIX, SOCK_DGRAM, 0);
  if (server_ < 0) {
    perror("Can't create file descriptor for the new socket.");
    exit(-1);
  }

  // call bind to associate the socket with the UNIX file system
  if (bind(server_, (sockaddr *) &server_addr_, sizeof(server_addr_)) < 0) {
    perror("Can't bind socket and server_addr.");
    exit(-1);
  }

  std::cout << "UNIX Server started on" + socket_filename_ << std::endl;
}

void UnixServer::Loop() {
  while (true) {
    std::string request = "";
    GetRequest(request);
    if (request.empty()) {
      continue;
    }

    std::cout << request;

    if (request.compare("Client is closed.\n") == 0) {
      break;
    }
  }
}

void UnixServer::GetRequest(std::string& request) {
  // read until we get a newline
  char buf[kBuffLen] = { 0 };

  struct pollfd fd;
  fd.fd = server_;
  fd.events = POLLIN;

  // wait for incomming data with timeout
  socklen_t len = sizeof(struct sockaddr_un);
  int ret = -1;
  do {
    ret = poll(&fd, 1, kPollTimeout);
  } while (ret == -1 && errno == EINTR);

  if (ret < 0 || fd.revents > 1) {
    perror("Socket error. poll()");
    return;
  } else if (ret == 0) {
    return;
  }

  // read data from socket
  int nread = -1;
  do {
    nread = recvfrom(server_, buf, kBuffLen, 0,
                     (struct sockaddr *) &server_addr_, &len);
  } while (nread == -1 && errno == EINTR);

  if (nread <= 0) {
    perror("Socket error, while receiving data from socket. recvfrom()");
    return;
  }

  // be sure to use append in case we have binary data
  request.append(buf, nread);

  if (request.find("\n") == std::string::npos) {
    request += "\n";
  }
}
