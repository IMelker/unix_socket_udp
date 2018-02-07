/*
 * unix_client.h
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#ifndef SOURCE_UNIX_CLIENT_H_
#define SOURCE_UNIX_CLIENT_H_


#include <sys/un.h>
#include <string>

class UnixClient {
 public:

  explicit UnixClient(const std::string& path);
  ~UnixClient();

  void Connect();   // fill sockaddr_un and init socket
  void Loop();      // start while cycle handling  std::cin

 private:
  void Send(const std::string& message);

  int client_;
  std::string socket_filename_;
  struct sockaddr_un server_addr_;
};

#endif /* SOURCE_UNIX_CLIENT_H_ */


