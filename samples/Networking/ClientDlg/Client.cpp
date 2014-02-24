//////////////////////////////////////////////
// Client.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "Client.h"


void CClientSocket::OnConnect()
{
	CClientDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_CONNECT, 0, 0);
}

void CClientSocket::OnDisconnect()
{
	CClientDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_DISCONNECT, 0, 0);
}

void CClientSocket::OnReceive()
{
	CClientDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->PostMessage(USER_RECEIVE, 0, 0);
}

