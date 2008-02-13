
#include "DialogApp.h"
#include "Client.h"


void CClient::OnClose()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientClose();
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

