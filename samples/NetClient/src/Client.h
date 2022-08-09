/////////////////////////////
// Client.h
//

#ifndef CLIENT_H
#define CLIENT_H

// User defined message
#define USER_ACCEPT     WM_APP+1
#define USER_CONNECT    WM_APP+2
#define USER_DISCONNECT WM_APP+3
#define USER_RECEIVE    WM_APP+4


///////////////////////////////////////////////////
// CClientSocket manages the network socket used by
// the client.
class CClientSocket : public CSocket
{
public:
    CClientSocket() {}
    virtual ~CClientSocket() {}

protected:
    // Virtual functions that override base class functions
    virtual void OnDisconnect();
    virtual void OnConnect();
    virtual void OnReceive();

private:
    CClientSocket(const CClientSocket&);                // Disable copy construction
    CClientSocket& operator = (const CClientSocket&);   // Disable assignment operator
};


#endif // CLIENT_H
