//////////////////////////////////////////////
// Client.h

#ifndef CLIENT_H
#define CLIENT_H


// User defined message
#define USER_ACCEPT     WM_APP+1
#define USER_CONNECT    WM_APP+2
#define USER_DISCONNECT	WM_APP+3
#define USER_RECEIVE    WM_APP+4


class CClientSocket : public CSocket
{
	virtual void OnDisconnect();
	virtual void OnConnect();
	virtual void OnReceive();
};


#endif // CLIENT_H





