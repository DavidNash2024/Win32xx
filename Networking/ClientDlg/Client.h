#ifndef CLIENT_H
#define CLIENT_H

#include "../../Win32++/Socket.h"


class CClient : public CSocket
{
	virtual void OnDisconnect();
	virtual void OnConnect();
	virtual void OnReceive();
};


#endif // CLIENT_H





