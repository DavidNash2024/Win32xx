/////////////////////////////
// Client.cpp
//

#include "stdafx.h"
#include "Client.h"
#include "DialogApp.h"

/////////////////////////////////////
// CClientSocket function definitions
//

// Called when a connection to the socket is made.
void CClientSocket::OnConnect()
{
    TRACE("The socket is connected\n");
    CClientDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_CONNECT, 0, 0);
}

// Called when the connection to the socket is disconnected.
void CClientSocket::OnDisconnect()
{
    TRACE("The socket is disconnected\n");
    CClientDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_DISCONNECT, 0, 0);
}

// Called when the socket has data to receive.
void CClientSocket::OnReceive()
{
    CClientDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_RECEIVE, 0, 0);
}

