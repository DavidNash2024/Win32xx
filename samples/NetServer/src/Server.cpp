//////////////////////////////////////////////
// Server.cpp

#include "stdafx.h"
#include "Server.h"
#include "DialogApp.h"



/////////////////////////////////////////
// Definitions of the CServerSocket class
//
void CServerSocket::OnDisconnect()
{
	CSvrDialog& Dialog = GetDlgApp().GetDialog();
	Dialog.PostMessage(USER_DISCONNECT, reinterpret_cast<WPARAM>(this), 0);
}

void CServerSocket::OnAccept()
{
	CSvrDialog& Dialog = GetDlgApp().GetDialog();
	Dialog.PostMessage(USER_ACCEPT, 0, 0);
}

void CServerSocket::OnReceive()
{
	CSvrDialog& Dialog = GetDlgApp().GetDialog();
	Dialog.PostMessage(USER_RECEIVE, reinterpret_cast<WPARAM>(this), 0);
}








