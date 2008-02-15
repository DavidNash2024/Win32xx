
#include "DialogApp.h"
#include "Client.h"


void CClient::OnDisconnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnClientDisconnect();
	PostMessage(Dialog, USER_DISCONNECT, 0, 0);
}

void CClient::OnConnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnClientConnect();
	PostMessage(Dialog, USER_CONNECT, 0, 0);
}

void CClient::OnReceive()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnClientReceive();
	PostMessage(Dialog, USER_RECEIVE, 0, 0);
}

