//////////////////////////////////////////////
// Client.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "Client.h"


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

