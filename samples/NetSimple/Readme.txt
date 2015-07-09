These two programs demonstrate the use of the CSocket class. They are both 
command line programs, to make them easier to understand.

Server.cpp
----------
This is a simple TCP server. It accepts a connection from a single TCP
client, with which it can exchange data. For simplicity, the server
is hardwired to the loopback address, allowing the server and client to
be tested on the same machine.

This program takes advantage of the notification events to detect when
there is data on the socket to be received.

Client.cpp
----------
This is the TCP client which connects to the server. The server must 
be running before the client is started for the client to connect.

This program also takes advantage of the notification events to detect when
there is data on the socket to be received.

