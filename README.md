# Unix UDP Socket programming example in C++

Example that show how to do UNIX UDP socket in C++. Use

> cmake

to compile the code.

## Simple UNIX UDP Server and Client

UNIX sockets use the file system to identify
the socket, rather than IP addresses and ports. In this code, I use
`/tmp/unix-socket` as the name of the socket.

The files `source/unix_client/unix_client.cpp` and
`source/unix_server/unix_server.cpp` contain
a basic server and client. The server handles all incomming data and 
simply redirect it to cout.

Run `unix_server` with `unix_client` and see what happens.

By using a UNIX socket, this code is restricted to communication with
local processes only. In addition, it is not portable to operating
systems that do not support UNIX sockets. This code is tested only on
Linux, but most of it should be portable to BSD and Mac OS X, with
some small changes.

Note that this code reuses the same base classes in `socket.h` and `client.h`,
showing how you can swap out a different type of socket, but the protocol
logic and processing remains the same.

