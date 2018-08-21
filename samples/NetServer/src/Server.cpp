//////////////////////////////////////////////
// Server.cpp

#include "stdafx.h"
#include "Server.h"
#include "DialogApp.h"


/////////////////////////////////////////
// Definitions of the CServerSocket class
//


void CServerSocket::OnAccept()
{
    CSvrDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_ACCEPT, 0, 0);
}


// Used for UDP only
void CServerSocket::OnReceive()
{
    CSvrDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_RECEIVE, reinterpret_cast<WPARAM>(this), 0);
}


/////////////////////////////////////////
// Definitions of the CWorkerSocket class
//


void CWorkerSocket::OnDisconnect()
{
    CSvrDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_DISCONNECT, reinterpret_cast<WPARAM>(this), 0);
}

void CWorkerSocket::OnReceive()
{
    CSvrDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_RECEIVE, reinterpret_cast<WPARAM>(this), 0);
}



