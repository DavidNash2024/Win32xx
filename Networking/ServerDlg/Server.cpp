

#include "stdafx.h"
#include "DialogApp.h"
#include "Server.h"


/////////////////////////////////////////
// Definitions of the CServerSocket class
//
void CServerSocket::OnDisconnect()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_DISCONNECT, (WPARAM)this, 0);
}

void CServerSocket::OnAccept()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_ACCEPT, 0, 0);
}

void CServerSocket::OnReceive()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_RECEIVE, (WPARAM)this, 0);
}








