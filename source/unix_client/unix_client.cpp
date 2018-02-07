/*
 * unix_client.cc
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */
#include "unix_client.h"

UnixClient::UnixClient(const std::string& path)
: socket_filename_(path),
  client_(-1){
  Connect();
}

UnixClient::~UnixClient() {
  Send("Client is closed.\n");
  close(client_);
}

void UnixClient::Connect() {
  memset(&server_addr_, 0, sizeof(struct sockaddr_un));
  server_addr_.sun_family = PF_UNIX;
  strncpy(server_addr_.sun_path, socket_filename_.c_str(),
          sizeof(server_addr_.sun_path) - 1);

  client_ = socket(PF_UNIX, SOCK_DGRAM, 0);
  if (!client_) {
    perror("Failed to initialize file descriptor for socket.");
    exit(-1);
  }
}

void UnixClient::Loop() {
  // read a line from standard input
  std::string line;
  while (std::getline(std::cin,line)) {
    if(line.compare("END")==0) {
      break;
    }
    Send(line);
  }
}

void UnixClient::Send(const std::string& message) {
  const char* ptr = message.c_str();
  unsigned long int nsize = message.length();
  int nwritten;

  do {
      nwritten = sendto(client_, ptr, nsize, 0, (struct sockaddr *) &server_addr_, sizeof(struct sockaddr_un));
    } while (nwritten == -1 && errno == EINTR);

  if (nwritten < 0) {
    perror("Can't write down to socket.");
    exit(-1);
  }
}
