#ifndef CLIENT_H
#define CLIENT_H

#include "../../Win32++/Socket.h"

class CClient : public CSocket
{
	virtual BOOL OnClose();
	virtual BOOL OnConnect();
	virtual BOOL OnReceive();
};


#endif // CLIENT_H





