NetClientAsync Sample
=====================
This code implements a network client. It supports TCP and UDP connections,
and support IPv4 and IPv6.

This code behaves much like the NetClient sample. The difference is the way
it handles network events. This NetClientAsync sample passes the network
events as messages to a window. The NetClient sample passes the network events
to a separate thread instead. 

To test the network, run the server application and listen on
a specific port (TCP or UDP). Then run the client application and connect 
to the server. If both the server and client are running on the same computer,
the client can connect to the loopback address (127.0.0.1). If they are running
on different computers the client will need to specify the correct address of 
the server. Note that a firewall on either the client or server's computer 
might block the network  connection.

To compile programs with CSocket, link with ws3_32.lib for Win32,
and ws2.lib for Windows CE. Windows 95 systems will need to install the
"Windows Sockets 2.0 for Windows 95". It's available from:
http:support.microsoft.com/kb/182108/EN-US/


Features demonstrated in this sample
====================================
* Implementing a TCP and UDP client
* Supporting IPv4 and IPv6
* Responding to networking events in a window procedure


About the CSocket class
=======================
The CSocket class represents a network socket. It encapsulates many of
the Windows Socket SPI functions, providing an object-oriented approach
to network programming. After StartAsync is called, network events are
passed as window messages to the specified window.

For a TCP server, create one instance of this class and use it as a listening socket.
The purpose of the listening socket is to detect connections from clients and accept them.
For the listening socket, we do the following:
 1) Create the socket.
 2) Bind an IP address to the socket.
 3) Listen on the socket for incoming connection requests.
 4) Use StartAsync to forward network events as messages to a window.
 5) Handle the message to accept requests on a newly created data CSocket object.
 6) Create a new data socket for each client connection accepted.

The purpose of the data socket is to send data to, and receive data from the client.
There will be one data socket for each client accepted by the server.
To use it we do the following:
 * To receive data from the client, override OnReceive and use Receive.
 * To send data to use Send.
 * OnDisconnect can be used to detect when the client is disconnected.

For a TCP client, inherit from CSocket and override OnReceive and OnDisconnect.
Create an instance of this inherited class, and  perform the following steps:
 1) Create the socket.
 2) Connect to the server.
 3) Use StartAsync to receive notifications as messages in the specified window.

We are now ready to send and receive data from the server.
 * Use Send to send data to the server.
 * Handle the Receive message to receive data from the server
 * Handle the Disconnect message to detect when the client is disconnected from the server.