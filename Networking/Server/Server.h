#ifndef SERVER_H
#define SERVER_H


#include "../../Win32++/Socket.h"

#define USER_ACCEPT		WM_APP+1
#define USER_CONNECT	WM_APP+2
#define USER_DISCONNECT	WM_APP+3
#define USER_RECEIVE	WM_APP+4


//////////////////////////////////////////////////////////////////////////////
// About the server socket classes ....                                      /
// There is only one instance of the CListenSocket class.                    /
// Its function is to listen for and accept connections from clients.        /
//                                                                           /
// This is one instance of the CClientSocket class per connected client.     /
// Their function is to send and recieve data between the server and client. /
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////
// Declaration of the CListenSocket class
//
class CListenSocket : public CSocket
{
public:
	virtual void OnAccept();
};


/////////////////////////////////////////
// Declaration of the CClientSocket class
//
class CClientSocket : public CSocket
{
public:
	virtual void OnDisconnect();
	virtual void OnReceive();
};



#endif // SERVER_H
