
#include "DialogApp.h"
#include "Client.h"


BOOL CClient::OnClose()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientClose();
	return TRUE;
}

BOOL CClient::OnConnect()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientConnect();
	return TRUE;
}

BOOL CClient::OnReceive()
{
	CClientDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientReceive();
	return TRUE;
}

