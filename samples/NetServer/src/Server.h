/////////////////////////////
// Server.h
//

#ifndef SERVER_H
#define SERVER_H


// User defined message
#define USER_ACCEPT     WM_APP+1
#define USER_CONNECT    WM_APP+2
#define USER_DISCONNECT WM_APP+3
#define USER_RECEIVE    WM_APP+4



// For a TCP server, inherit a class from CSocket and override OnAccept, OnDisconnect
// and OnRecieve. Create one instance of this class and use it as a listening socket.
// The purpose of the listening socket is to detect connections from clients and accept them.
// For the listening socket, we do the following:
// 1) Create the socket.
// 2) Bind an IP address to the socket.
// 3) Listen on the socket for incoming connection requests.
// 4) Use StartNotifyRevents to receive notification of network events.
// 5) Override OnAccept and Accept requests on a newly created data CSocket object.
//
// The purpose of the data socket is to send data to, and receive data from the client.
// There will be one data socket for each client accepted by the server.
// It is already set up ready for use by Accept. To use it we do the following:
// * To recieve data from the client, override OnReceive and use Receive.
// * To send data to use Send.
// * OnDisconnect can be used to detect when the client is disconnected.

// For a TCP client, inherit from CSocket and override OnReceive and OnDisconnect.
// Create an instance of this inherited class, and  perform the following steps:
// 1) Create the socket.
// 2) Connect to the server.
// 3) Use StartNotifyRevents to receive notification of network events.
//    We are now ready to send and receive data from the server.
// * Use Send to send data to the server.
// * Override OnReceive and use Receive to receive data from the server
// * OnDisconnect can be used to detect when the client is disconnected from the server.


///////////////////////////////////////////////////
// CServerSocket manages the network socket used by
// the client. This socket listens for connection
// requests from clients.
//
class CServerSocket : public CSocket
{
public:
    CServerSocket() { }
    virtual ~CServerSocket() { }

protected:
    // Virtual functions that override base class functions
    virtual void OnAccept();
    virtual void OnReceive();
};


///////////////////////////////////////////
// CWorkerSocket manages the worker socket.
// A worker socket is created for each TCP
// client that connects to the server.
class CWorkerSocket : public CSocket
{
public:
    CWorkerSocket() { }
    virtual ~CWorkerSocket() { }

protected:
    // Virtual functions that override base class functions
    virtual void OnDisconnect();
    virtual void OnReceive();
};


#endif // SERVER_H
