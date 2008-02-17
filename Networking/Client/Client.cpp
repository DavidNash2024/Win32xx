
#include "DialogApp.h"
#include "Client.h"


void CClient::OnDisconnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientDisconnect();
}

void CClient::OnConnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientConnect();
}

void CClient::OnReceive()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientReceive();
}

