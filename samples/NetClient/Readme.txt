Networking Examples
===================
This folder contains several projects demonstrating the networking features
of Win32++. To test the network, run the server application and listen on
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


Features demonstrated in these examples
=======================================
* Implementing a TCP and UDP server
* Implementing a TCP and UDP client
* Responding to networking events
* Multi-threaded programming


About the CSocket class
=======================
The CSocket class represents a network socket. It encapsulates many of
the Windows Socket SPI functions, providing an object-oriented approach
to network programming. After StartEvents is called, CSocket monitors
the socket and responds automatically to network events. This event
monitoring, for example, automatically calls OnReceive when there is
data on the socket to be read, and OnAccept when a server should accept
a connection from a client.

Users of this class should be aware that functions like OnReceive,
OnAccept, etc. are called on a different thread from the one CSocket is
instantiated on. The thread for these functions needs to respond quickly
to other network events, so it shouldn't be delayed. It also doesn't run
a message loop, so it can't be used to create windows. For these reasons
it might be best to use PostMessage in response to these functions in a
windows environment.

For a TCP server, inherit a class from CSocket and override OnAccept, OnDisconnect
and OnRecieve. Create one instance of this class and use it as a listening socket.
The purpose of the listening socket is to detect connections from clients and accept them.
For the listening socket, we do the following:
 1) Create the socket.
 2) Bind an IP address to the socket.
 3) Listen on the socket for incoming connection requests.
 4) Use StartNotifyRevents to receive notification of network events.
 5) Override OnAccept and Accept requests on a newly created data CSocket object.
 6) Create a new data socket for each client connection accepted.
 7) The server socket uses the 'accept' function to accept an incoming connection 
     from this new data socket.

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
 3) Use StartNotifyRevents to receive notification of network events.

We are now ready to send and receive data from the server.
 * Use Send to send data to the server.
 * Override OnReceive and use Receive to receive data from the server
 * OnDisconnect can be used to detect when the client is disconnected from the server.