//////////////////////////////////////////////
// Client.cpp

#include "stdafx.h"
#include "Client.h"
#include "DialogApp.h"


void CClientSocket::OnConnect()
{
	TRACE("The socket is connected\n");
    CClientDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_CONNECT, 0, 0);
}

void CClientSocket::OnDisconnect()
{
	TRACE("The socket is disconnected\n");
    CClientDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_DISCONNECT, 0, 0);
}

void CClientSocket::OnReceive()
{
    CClientDialog& dialog = GetDlgApp().GetDialog();
    dialog.PostMessage(USER_RECEIVE, 0, 0);
}

