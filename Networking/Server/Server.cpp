
#include "DialogApp.h"
#include "Server.h"

/////////////////////////////////////////
// Definitions of the CClientSocket class
//
void CClientSocket::OnDisconnect()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	PostMessage(Dialog, USER_DISCONNECT, (WPARAM)this, 0);
//	Dialog.OnClientDisconnect(this);
}

void CClientSocket::OnReceive()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnClientReceive(this);
	PostMessage(Dialog, USER_RECEIVE, (WPARAM)this, 0);
}


/////////////////////////////////////////
// Definitions of the CListenSocket class
//
void CListenSocket::OnAccept()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnListenAccept();
	PostMessage(Dialog, USER_ACCEPT, 0, 0);
}





