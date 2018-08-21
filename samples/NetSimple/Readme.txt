These two programs demonstrate the use of the CSocket class. They are both 
command line programs, to make them easier to understand.

Server.cpp
----------
This is a simple UDP server. It accepts a connection from a multiple UDP
clients, with which it can exchange data. The server can't send data until
it receives data from a client.The server will send data to the UPD client
from which it most recently received data. 

For simplicity, the server is hardwired to the loopback address, allowing
the server and client to be tested on the same machine. 

This program takes advantage of the notification events to detect when
there is data on the socket to be received.

Client.cpp
----------
This is the UDP client which connects to the server. The server must 
be running before the client is started for the client to connect.

This program also takes advantage of the notification events to detect when
there is data on the socket to be received.

You can run multiple copies of the client to connect to the server.

