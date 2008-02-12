
#include "DialogApp.h"
#include "Server.h"


BOOL CClientSocket::OnClose()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientClose();
	return TRUE;
}

BOOL CClientSocket::OnReceive()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientReceive(this);
	return TRUE;
}




BOOL CListenSocket::OnAccept()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnListenAccept();
	return TRUE;
}





