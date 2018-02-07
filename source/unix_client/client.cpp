/*
 * client.cpp
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#include "unix_client.h"

int main(int argc, char **argv) {
  UnixClient client = UnixClient("/tmp/unix_sock");
  client.Loop();
}
