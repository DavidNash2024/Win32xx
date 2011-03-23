//////////////////////////////////////////////
// Client.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "Client.h"


void CClientSocket::OnConnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_CONNECT, 0, 0);
}

void CClientSocket::OnDisconnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_DISCONNECT, 0, 0);
}

void CClientSocket::OnReceive()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_RECEIVE, 0, 0);
}

