
#include "DialogApp.h"
#include "Server.h"


/////////////////////////////////////////
// Definitions of the CServerSocket class
//
void CServerSocket::OnDisconnect()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();

// Note: We must post a message in response to this event, because
//	      CSvrDialog::OnSocketDisconnet will destroy this CServerSocket
//	      object.

	PostMessage(Dialog, USER_DISCONNECT, (WPARAM)this, 0);
}

void CServerSocket::OnAccept()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
//	Dialog.OnSocketAccept();
	PostMessage(Dialog, USER_ACCEPT, 0, 0);
}

void CServerSocket::OnReceive()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnSocketReceive(this);
}








