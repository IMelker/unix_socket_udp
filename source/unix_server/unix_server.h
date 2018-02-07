/*
 * unix_server.h
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#ifndef SOURCE_UNIX_SERVER_H_
#define SOURCE_UNIX_SERVER_H_

#include <sys/un.h>
#include <string>

class UnixServer {
 public:
  explicit UnixServer(const std::string& path);
  ~UnixServer();

  void Create();  // fill sockaddr_un, init socket and bind it
  void Loop();  // start while cycle handling GetRequest()

 private:
  void GetRequest(std::string& request);  // get data from socket and write it to "request"

  std::string socket_filename_;
  int server_;
  struct sockaddr_un server_addr_;
};

#endif /* SOURCE_UNIX_SERVER_H_ */
