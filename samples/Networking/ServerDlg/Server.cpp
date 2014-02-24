//////////////////////////////////////////////
// Server.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "Server.h"


/////////////////////////////////////////
// Definitions of the CServerSocket class
//
void CServerSocket::OnDisconnect()
{
	CSvrDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_DISCONNECT, (WPARAM)this, 0);
}

void CServerSocket::OnAccept()
{
	CSvrDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_ACCEPT, 0, 0);
}

void CServerSocket::OnReceive()
{
	CSvrDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_RECEIVE, (WPARAM)this, 0);
}








