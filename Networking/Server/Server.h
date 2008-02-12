#ifndef SERVER_H
#define SERVER_H


#include "../../Win32++/Socket.h"



class CClientSocket : public CSocket
{
public:
	virtual BOOL OnClose();
	virtual BOOL OnReceive();

};

class CListenSocket : public CSocket
{
public:
	virtual BOOL OnAccept();

};


#endif // SERVER_H
