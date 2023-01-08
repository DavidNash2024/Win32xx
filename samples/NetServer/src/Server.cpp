/////////////////////////////
// Server.cpp
//

#include "stdafx.h"
#include "Server.h"
#include "DialogApp.h"


/////////////////////////////////////
// CServerSocket funtion definitions
//

// Called when the socket accepts a connection.
void CServerSocket::OnAccept()
{
    CSvrDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_ACCEPT, 0, 0);
}

// Called when the socket receives data.
// Used for UDP only.
void CServerSocket::OnReceive()
{
    CSvrDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_RECEIVE, reinterpret_cast<WPARAM>(this), 0);
}


/////////////////////////////////////
// CWorkerSocket function definitions
//

// Called when the socket is disconnected.
void CWorkerSocket::OnDisconnect()
{
    CSvrDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_DISCONNECT, reinterpret_cast<WPARAM>(this), 0);
}

// Called when the socket receives data.
void CWorkerSocket::OnReceive()
{
    CSvrDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_RECEIVE, reinterpret_cast<WPARAM>(this), 0);
}

