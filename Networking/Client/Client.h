#ifndef CLIENT_H
#define CLIENT_H

#include "../../Win32++/Socket.h"

class CClient : public CSocket
{
	virtual void OnClose();
	virtual void OnConnect();
	virtual void OnReceive();
};


#endif // CLIENT_H





