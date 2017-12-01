//////////////////////////////////////////////
// Client.cpp

#include "stdafx.h"
#include "Client.h"
#include "DialogApp.h"


void CClientSocket::OnConnect()
{
    CClientDialog& Dialog = GetDlgApp().GetDialog();
    Dialog.PostMessage(USER_CONNECT, 0, 0);
}

void CClientSocket::OnDisconnect()
{
    CClientDialog& Dialog = GetDlgApp().GetDialog();
    Dialog.PostMessage(USER_DISCONNECT, 0, 0);
}

void CClientSocket::OnReceive()
{
    CClientDialog& Dialog = GetDlgApp().GetDialog();
    Dialog.PostMessage(USER_RECEIVE, 0, 0);
}

