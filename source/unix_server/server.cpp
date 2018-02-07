/*
 * server.cpp
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#include "unix_server.h"

int main(int argc, char **argv) {
  UnixServer server = UnixServer("/tmp/unix_sock");
  server.Loop();
}
